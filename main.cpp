#define WLR_USE_UNSTABLE
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/managers/KeybindManager.hpp>
#include <hyprland/src/managers/input/InputManager.hpp>

extern "C" {

// --- API Version ------------------------------------------------------------
APICALL EXPORT const char* PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

// --- Globals ----------------------------------------------------------------
static HANDLE g_handle = nullptr;

// --- Plugin Init ------------------------------------------------------------
APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    g_handle = handle;

    HyprlandAPI::addNotification(
        g_handle,
        "[csd-titlebar-move] Plugin loaded successfully!",
        CHyprColor{0.2, 0.8, 0.2, 1.0},
        3000
    );

    return {
        .name = "csd-titlebar-move",
        .description = "Move GTK CSD windows by titlebar (safe minimal build)",
        .author = "bennch",
        .version = "0.4.2"
    };
}

// --- Plugin Exit ------------------------------------------------------------
APICALL EXPORT void PLUGIN_EXIT() {
    HyprlandAPI::addNotification(
        g_handle,
        "[csd-titlebar-move] Plugin unloaded",
        CHyprColor{0.6, 0.6, 0.6, 1.0},
        2000
    );
}

} // extern "C"
