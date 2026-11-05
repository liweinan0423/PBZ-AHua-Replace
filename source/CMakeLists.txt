cmake_minimum_required(VERSION 3.24)
project(AHuaReplace LANGUAGES CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/utf-8>)

set(REF "${CMAKE_CURRENT_SOURCE_DIR}/../../reference/CringeBladeZero/src" CACHE PATH "CringeBladeZero src directory")

# The toolkit includes headers by bare name.
include_directories(
    ${REF}/core ${REF}/ui ${REF}/boss ${REF}/grab ${REF}/air
    ${REF}/weapons ${REF}/character ${REF}/hair ${REF}/pad
    ${REF}/supplement ${REF}/generated)

add_library(version SHARED
    dllmain.cpp
    stubs.cpp
    ${REF}/core/ue_runtime.cpp
    ${REF}/core/game_thread.cpp
    ${REF}/core/settings_store.cpp
    ${REF}/character/npc_retarget.cpp
    ${REF}/core/forwarders.asm
    version.def)

target_compile_definitions(version PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
# Upstream uses /W4 /WX; the trimmed target keeps the diagnostics but not the
# errors so a newer MSVC can still build it.
target_compile_options(version PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/W3;/EHsc;/wd4244;/wd4267;/wd4305>")
target_link_libraries(version PRIVATE bcrypt user32)

set_target_properties(version PROPERTIES
    OUTPUT_NAME version
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin_debug)
