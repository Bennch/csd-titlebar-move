#define WLR_USE_UNSTABLE

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/managers/KeybindManager.hpp>

#include "globals.hpp"

// Methods
void onClick(SCallbackInfo& info, IPointer::SButtonEvent e) {
    if (e.state == WL_POINTER_BUTTON_STATE_RELEASED)
        if (!info.cancelled)
            g_pKeybindManager->m_mDispatchers["mouse"]("0movewindow");
}

void hkCXDGToplevelResConstructor(CXDGToplevelResource* thisptr, SP<CXdgToplevel> resource, SP<CXDGSurfaceResource> owner) {
    (*(origCXDGToplevelResConstructor)g_pCXDGToplevelResConstructor->m_pOriginal)(thisptr, resource, owner);

    resource->setMove([thisptr](CXdgToplevel* t, wl_resource* seat, uint32_t serial) {
        // treat it as down event, moustButton event is used for up event only
        if (auto window = thisptr->window.lock()) {
            g_pCompositor->focusWindow(window);
            g_pKeybindManager->m_mDispatchers["mouse"]("1movewindow");
        }
    });
}

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    const std::string HASH = __hyprland_api_get_hash();

    if (HASH != GIT_COMMIT_HASH) {
        HyprlandAPI::addNotification(PHANDLE,
                                     "[csd-titlebar-move] Failure in initialization: Version mismatch "
                                     "(headers ver is not equal to running hyprland ver)",
                                     CHyprColor{1.0, 0.2, 0.2, 1.0}, 5000);
        throw std::runtime_error("[csd-titlebar-move] Version mismatch");
    }

    auto FNS = HyprlandAPI::findFunctionsByName(PHANDLE, "CXDGToplevelResource");
    for (auto& fn : FNS) {
        if (!fn.demangled.contains("CXDGToplevelResource::CXDGToplevelResource"))
            continue;
        g_pCXDGToplevelResConstructor = HyprlandAPI::createFunctionHook(PHANDLE, fn.address, (void*)::hkCXDGToplevelResConstructor);
        break;
    }

    if (!g_pCXDGToplevelResConstructor) {
        HyprlandAPI::addNotification(PHANDLE,
                                     "[csd-titlebar-move] Failure in initialization: Failed to find "
                                     "required hook fns",
                                     CHyprColor{1.0, 0.2, 0.2, 1.0}, 5000);
        throw std::runtime_error("[csd-titlebar-move] Hooks fn init failed");
    }

    g_pCXDGToplevelResConstructor->hook();

    if (g_pCXDGToplevelResConstructor)
        HyprlandAPI::addNotification(PHANDLE, "[csd-titlebar-move] Initialized successfully!", CHyprColor{0.2, 1.0, 0.2, 1.0}, 5000);
    else {
        HyprlandAPI::addNotification(PHANDLE, "[csd-titlebar-move] Failure in initialization (hook failed)!", CHyprColor{1.0, 0.2, 0.2, 1.0}, 5000);
        throw std::runtime_error("[csd-titlebar-move] Hooks failed");
    }

    return {"csd-titlebar-move", "A plugin to move CSD GTK applications by their titlebar", "khalid151", "0.1"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    ;
}
