#define WLR_USE_UNSTABLE
#include <any>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include "globals.hpp"

void onClick(SCallbackInfo& info, const IPointer::SButtonEvent& e) {
    if (e.button == BTN_LEFT && e.state == WL_POINTER_BUTTON_STATE_RELEASED) {
        if (!info.cancelled && g_pKeybindManager) {
            HyprlandAPI::addNotification(
                PHANDLE,
                "[csd-titlebar-move] moving window",
                CHyprColor{0.3, 0.9, 0.3, 1.0},
                1000
            );
            g_pKeybindManager->m_dispatchers["mouse"]("0movewindow");
        }
    }
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO pluginInit(HANDLE handle) {
    PHANDLE = handle;

    HyprlandAPI::addNotification(
        PHANDLE,
        "[csd-titlebar-move] initialized (Aquamarine API)",
        CHyprColor{0.2, 1.0, 0.2, 1.0},
        3000
    );

    // neue Signatur: void*(self), SCallbackInfo&, std::any
    g_pMouseBtnCallback = HyprlandAPI::registerCallbackDynamic(
        PHANDLE, "mouseButton",
        [](void* self, SCallbackInfo& info, std::any data) {
            try {
                const auto& e = std::any_cast<IPointer::SButtonEvent>(data);
                onClick(info, e);
            } catch (const std::bad_any_cast&) {
                HyprlandAPI::addNotification(
                    PHANDLE,
                    "[csd-titlebar-move] bad_any_cast in mouseButton callback",
                    CHyprColor{1.0, 0.3, 0.3, 1.0},
                    2000
                );
            }
        }
    );

    return {
        .name = "csd-titlebar-move",
        .description = "Move GTK CSD windows by titlebar (Aquamarine)",
        .author = "khalid151 (patched)",
        .version = "0.3.1"
    };
}

APICALL EXPORT void pluginExit() {
    g_pMouseBtnCallback.reset();
    HyprlandAPI::addNotification(
        PHANDLE,
        "[csd-titlebar-move] unloaded",
        CHyprColor{0.7, 0.7, 1.0, 1.0},
        2000
    );
}
