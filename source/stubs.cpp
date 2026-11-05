// Minimal stand-ins for the toolkit modules that NpcRetarget references but the
// standalone AHua mod does not ship (grabs, audit dummy, skill toast, hotkeys,
// boss-entry protection). They keep the retarget/apply path intact.

#include <string>

#include "motian_grab.h"
#include "grab_extras.h"
#include "skill_toast.h"
#include "ui_hotkeys.h"
#include "entry_protection.h"

namespace MotianGrab {
Snapshot GetSnapshot() { return {}; }
bool RequestOptions(const Options&) { return false; }
bool RequestTrigger() { return false; }
bool RequestTrigger(int32_t) { return false; }
void RequestCancel() {}
void OnGameThreadGuarded() {}
bool BeforeTravel() { return true; }
bool HandleCommand(const char*, const char*) { return false; }
}  // namespace MotianGrab

namespace GrabExtras {
bool Active() { return false; }
bool ChangeHealth(unsigned char*, float) { return false; }
bool ChangeMaxHealth(unsigned char*, float) { return false; }
}  // namespace GrabExtras

namespace SkillToast {
void Show(const std::wstring&, const std::wstring&) {}
void Tick() {}
bool Active() { return false; }
void Shutdown() {}
}  // namespace SkillToast

namespace UiHotkeys {
std::wstring Hint(Action) { return L""; }
}  // namespace UiHotkeys

namespace EntryProtection {
void Initialize(Logger) {}
bool Protect(ue::Object*) { return false; }
bool ReleaseProtection() { return true; }
bool Pause(ue::Object*, ue::Object*, bool) { return false; }
bool ReleasePause(ue::Object*) { return false; }
Snapshot GetSnapshot() { return {}; }
bool HasProtection() { return false; }
}  // namespace EntryProtection
