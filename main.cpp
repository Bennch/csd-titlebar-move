#define WLR_USE_UNSTABLE
#include <any>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/managers/KeybindManager.hpp>
#include <hyprland/src/managers/input/InputManager.hpp>

// -----------------------------------------------------------------------------
//  Hyprland Plugin: CSD Titlebar Move (Aquamarine-compatible)
// -----------------------------------------------------------------------------

extern "C" {

// Hyprland will call this function to verify ABI compatibility
APICALL EXPORT const char* PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

// global handles
static HANDLE g_handle = nullptr;
static SP<HOOK_CALLBACK_FN> g_cb;

// forward declarations
static void onClick(SCallbackInfo& info, const IPointer::SButtonEvent& e);

// -----------------------------------------------------------------------------
//  Init
// -----------------------------------------------------------------------------
APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    g_handle = handle;

    g_cb = HyprlandAPI::registerCallbackDynamic(
        g_handle,
        "mouseButton",
        [](void* self, SCallbackInfo& info, std::any data) {
            try {
                const auto& e = std::any_cast<IPointer::SButtonEvent>(data);
                onClick(info, e);
            } catch (const std::bad_any_cast&) {
                HyprlandAPI::addNotification(
                    g_handle,
                    "[csd-titlebar-move] bad_any_cast in mouseButton callback",
                    CHyprColor{1.0, 0.3, 0.3, 1.0},
                    2000
                );
            }
        }
    );

    return {
        .name = "csd-titlebar-move",
        .description = "Move CSD windows by dragging titlebar (updated for v0.51+)",
        .author = "bennch",
        .version = "0.4.1"
    };
}

// -----------------------------------------------------------------------------
//  Exit
// -----------------------------------------------------------------------------
APICALL EXPORT void PLUGIN_EXIT() {
    g_cb.reset();
}

// -----------------------------------------------------------------------------
//  Logic
// -----------------------------------------------------------------------------
static void onClick(SCallbackInfo& info, const IPointer::SButtonEvent& e) {
    if (e.button == BTN_LEFT && e.state == WL_POINTER_BUTTON_STATE_RELEASED && !info.cancelled) {
        if (g_pKeybindManager) {
            g_pKeybindManager->m_dispatchers["movewindow"]("");
        }
    }
}

} // extern "C"
