#pragma once

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/managers/KeybindManager.hpp>
#include <hyprland/src/render/Renderer.hpp>
#include <hyprland/src/helpers/Color.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

// Handle für Plugin API
inline HANDLE PHANDLE = nullptr;

// Callback-Objekt für Mausereignisse
inline SP<HOOK_CALLBACK_FN> g_pMouseBtnCallback = nullptr;

// Zugriff auf globale Manager direkt (werden von Hyprland bereitgestellt)
extern UP<CCompositor> g_pCompositor;
extern UP<CKeybindManager> g_pKeybindManager;
