#include <windows.h>
#include <bcrypt.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <string>

#include "exports.generated.h"
#include "ue_runtime.h"
#include "game_thread.h"
#include "npc_retarget.h"
#include "settings_store.h"

namespace {

HMODULE self_module = nullptr;
INIT_ONCE system_once = INIT_ONCE_STATIC_INIT;
std::array<FARPROC, kExportCount> system_functions{};
volatile LONG stop_worker = 0;
volatile LONG worker_status = 0;
wchar_t log_path[MAX_PATH]{};

// Pinned build: Phantom Blade Zero 2026-11-04 (Steam build 26367565).
constexpr DWORD kTimestamp = 0x6ae7ae3f;
constexpr DWORD kImageSize = 0x1e0bb000;
constexpr DWORD kEntrypoint = 0x18db0d40;
constexpr uint64_t kExeBytes = 498913272;
constexpr const char* kSha256 = "4415933ce33476f14c837abb61a1b60af8b78cd81783896501913a7aac69e423";

void Log(const char* message) {
    if (!log_path[0]) return;
    SYSTEMTIME t{}; GetLocalTime(&t);
    char line[1024]{};
    int n = _snprintf_s(line, sizeof(line), _TRUNCATE,
        "%04u-%02u-%02u %02u:%02u:%02u pid=%lu %s\r\n",
        t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond,
        GetCurrentProcessId(), message);
    if (n < 0) { n = static_cast<int>(sizeof(line)) - 1; line[n - 2] = '\r'; line[n - 1] = '\n'; }
    if (n <= 0) return;
    HANDLE file = CreateFileW(log_path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return;
    LARGE_INTEGER size{};
    if (GetFileSizeEx(file, &size) && size.QuadPart >= 1024 * 1024) {
        CloseHandle(file);
        const std::wstring previous = std::wstring(log_path) + L".previous";
        if (!MoveFileExW(log_path, previous.c_str(), MOVEFILE_REPLACE_EXISTING)) return;
        file = CreateFileW(log_path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file == INVALID_HANDLE_VALUE) return;
    }
    DWORD written = 0; WriteFile(file, line, static_cast<DWORD>(n), &written, nullptr); CloseHandle(file);
}

void LogLine(const char* message) { Log(message); }

ULONG_PTR WINAPI MissingExport() {
    SetLastError(ERROR_PROC_NOT_FOUND);
    return 0;
}

BOOL CALLBACK LoadSystem(PINIT_ONCE, PVOID, PVOID*) {
    wchar_t directory[MAX_PATH]{};
    const UINT n = GetSystemDirectoryW(directory, MAX_PATH);
    if (n > 0 && n + 13 < MAX_PATH) {
        wcsncat_s(directory, L"\\version.dll", _TRUNCATE);
        HMODULE real = LoadLibraryExW(directory, nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (real && real != self_module)
            for (size_t i = 0; i < kExportCount; ++i)
                system_functions[i] = GetProcAddress(real, kExportNames[i]);
    }
    for (auto& fn : system_functions)
        if (!fn) fn = reinterpret_cast<FARPROC>(&MissingExport);
    return TRUE;
}

bool Read(uintptr_t address, void* dest, size_t bytes) {
    if (address < 0x10000 || address > UINTPTR_MAX - bytes) return false;
    SIZE_T n = 0;
    return ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<const void*>(address),
        dest, bytes, &n) != FALSE && n == bytes;
}
template<class T> bool ReadValue(uintptr_t p, T& out) { return Read(p, &out, sizeof(out)); }

bool HashExecutable(const wchar_t* path) {
    HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    LARGE_INTEGER size{};
    if (!GetFileSizeEx(file, &size) || static_cast<uint64_t>(size.QuadPart) != kExeBytes) { CloseHandle(file); return false; }
    BCRYPT_ALG_HANDLE algorithm = nullptr; BCRYPT_HASH_HANDLE hash = nullptr;
    bool valid = false;
    if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0) >= 0 &&
        BCryptCreateHash(algorithm, &hash, nullptr, 0, nullptr, 0, 0) >= 0) {
        std::array<unsigned char, 1 << 20> buffer{};
        DWORD count = 0; bool ok = true;
        while (true) {
            if (!ReadFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &count, nullptr)) { ok = false; break; }
            if (!count) break;
            if (BCryptHashData(hash, buffer.data(), count, 0) < 0) { ok = false; break; }
        }
        std::array<unsigned char, 32> digest{};
        if (ok && BCryptFinishHash(hash, digest.data(), static_cast<ULONG>(digest.size()), 0) >= 0) {
            char hex[65]{};
            for (size_t i = 0; i < digest.size(); ++i) _snprintf_s(hex + i * 2, 3, _TRUNCATE, "%02x", digest[i]);
            valid = std::strcmp(hex, kSha256) == 0;
        }
    }
    if (hash) BCryptDestroyHash(hash);
    if (algorithm) BCryptCloseAlgorithmProvider(algorithm, 0);
    CloseHandle(file);
    return valid;
}

bool KnownBuild(uintptr_t base, const wchar_t* executable) {
    IMAGE_DOS_HEADER dos{}; IMAGE_NT_HEADERS64 nt{};
    if (!ReadValue(base, dos) || dos.e_magic != IMAGE_DOS_SIGNATURE || dos.e_lfanew < 0 || dos.e_lfanew > 1024 * 1024 ||
        !ReadValue(base + static_cast<uintptr_t>(dos.e_lfanew), nt)) return false;
    if (nt.Signature != IMAGE_NT_SIGNATURE || nt.FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 ||
        nt.FileHeader.TimeDateStamp != kTimestamp || nt.OptionalHeader.SizeOfImage != kImageSize ||
        nt.OptionalHeader.AddressOfEntryPoint != kEntrypoint) return false;
    return HashExecutable(executable);
}

// NpcRetarget asks this before taking the player's appearance. The standalone
// mod owns the appearance, so nothing else has to be released.
bool ReleaseAppearance() { return true; }

// Applied once the hero exists; the request is retried until NpcRetarget
// reports the AHua skin active.
void DispatchGameThread() {
    NpcRetarget::OnGameThreadGuarded();

    static ULONGLONG next_try = 0;
    static bool done = false;
    if (done) return;

    const ULONGLONG now = GetTickCount64();
    if (now < next_try) return;
    next_try = now + 2000;

    const NpcRetarget::Snapshot state = NpcRetarget::GetSnapshot();
    if (state.active && state.selected.find(L"\u963f\u82b1") != std::wstring::npos) {
        done = true;
        Log("AHUA_ACTIVE");
        return;
    }
    if (!state.ready) return;
    if (!state.experimental) NpcRetarget::SetExperimental(true);

    int32_t index = -1;
    for (const auto& entry : state.entries) {
        if (entry.id == "AHua") { index = entry.index; break; }
    }
    if (index < 0) { Log("AHUA_PROFILE_MISSING"); done = true; return; }
    NpcRetarget::Request(NpcRetarget::Action::Apply, index);
}

void FaultHandler(DWORD code, uintptr_t address, DWORD faults, bool disabled) {
    char line[220]{};
    _snprintf_s(line, sizeof(line), _TRUNCATE,
        "GAME_THREAD fault %lu/%lu code=0x%08lx address=%p disabled=%d",
        faults, GameThread::kMaxFaults, code, reinterpret_cast<void*>(address), disabled ? 1 : 0);
    Log(line);
}

DWORD WINAPI Worker(void*) {
    HMODULE pinned = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        reinterpret_cast<LPCWSTR>(&Worker), &pinned)) return 0;

    if (GetModuleFileNameW(self_module, log_path, MAX_PATH)) {
        if (wchar_t* slash = wcsrchr(log_path, L'\\'))
            wcsncpy_s(slash + 1, MAX_PATH - (slash + 1 - log_path), L"AHuaReplace.log", _TRUNCATE);
        else log_path[0] = 0;
    }

    wchar_t executable[MAX_PATH]{};
    GetModuleFileNameW(nullptr, executable, MAX_PATH);
    const uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (!KnownBuild(base, executable)) { Log("UNSUPPORTED_BUILD; no data writes"); return 0; }
    Log("KNOWN_BUILD sha256 verified");
    InterlockedExchange(&worker_status, 1);

    ue::Attach(base);

    wchar_t ini[MAX_PATH]{};
    wcsncpy_s(ini, log_path, _TRUNCATE);
    if (wchar_t* dot = wcsrchr(ini, L'.')) wcscpy_s(dot, 8, L".ini");
    SettingsStore::Initialize(ini);
    NpcRetarget::Initialize(&LogLine, &ReleaseAppearance);
    Log("NPC_RETARGET initialized; waiting for the hero");

    bool hook_attempted = false;
    while (!InterlockedCompareExchange(&stop_worker, 0, 0)) {
        if (!hook_attempted) {
            if (ue::Object* engine = ue::FindEngine()) {
                char why[160]{};
                if (GameThread::Install(engine, base + ue::layout::kGameEngineTick,
                        &DispatchGameThread, &FaultHandler, why, sizeof(why))) {
                    hook_attempted = true;
                    InterlockedExchange(&worker_status, 2);
                    Log("GAME_THREAD_HOOK installed; AHua auto-apply armed");
                } else {
                    Log(why);
                }
            }
        }
        Sleep(50);
    }
    return 0;
}

}  // namespace

extern "C" FARPROC ResolveVersionExport(unsigned int index) {
    const DWORD saved_error = GetLastError();
    InitOnceExecuteOnce(&system_once, LoadSystem, nullptr, nullptr);
    FARPROC answer = index < system_functions.size() ? system_functions[index] : reinterpret_cast<FARPROC>(&MissingExport);
    SetLastError(saved_error);
    return answer;
}

extern "C" DWORD WINAPI AHuaReplaceStatus() { return static_cast<DWORD>(InterlockedCompareExchange(&worker_status, 0, 0)); }

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        self_module = module;
        HANDLE thread = CreateThread(nullptr, 0, Worker, nullptr, 0, nullptr);
        if (thread) CloseHandle(thread);
    } else if (reason == DLL_PROCESS_DETACH) {
        InterlockedExchange(&stop_worker, 1);
    }
    return TRUE;
}
