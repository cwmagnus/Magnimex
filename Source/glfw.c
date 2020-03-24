// Includes GLFW source files so glfw can be compiled into the library

#if defined(_WIN32)
#define _GLFW_WIN32
#endif
#if defined(__linux__)
#if !defined(_GLFW_WAYLAND)
#define _GLFW_X11
#endif
#endif
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#define _GLFW_X11
#endif
#if defined(__APPLE__)
#define _GLFW_COCOA
#define _GLFW_USE_MENUBAR
#define _GLFW_USE_RETINA
#endif
#if defined(__TINYC__)
#define _WIN32_WINNT_WINXP      0x0501
#endif

#include "../External/glfw/src/context.c"
#include "../External/glfw/src/init.c"
#include "../External/glfw/src/input.c"
#include "../External/glfw/src/monitor.c"
#include "../External/glfw/src/vulkan.c"
#include "../External/glfw/src/window.c"

#if defined(_WIN32)
#include "../External/glfw/src/win32_init.c"
#include "../External/glfw/src/win32_joystick.c"
#include "../External/glfw/src/win32_monitor.c"
#include "../External/glfw/src/win32_time.c"
#include "../External/glfw/src/win32_thread.c"
#include "../External/glfw/src/win32_window.c"
#include "../External/glfw/src/wgl_context.c"
#include "../External/glfw/src/egl_context.c"
#include "../External/glfw/src/osmesa_context.c"
#endif

#if defined(__linux__)
#if defined(_GLFW_WAYLAND)
#include "../External/glfw/src/wl_init.c"
#include "../External/glfw/src/wl_monitor.c"
#include "../External/glfw/src/wl_window.c"
#include "../External/glfw/src/wayland-pointer-constraints-unstable-v1-client-protocol.c"
#include "../External/glfw/src/wayland-relative-pointer-unstable-v1-client-protocol.c"
#endif
#if defined(_GLFW_X11)
#include "../External/glfw/src/x11_init.c"
#include "../External/glfw/src/x11_monitor.c"
#include "../External/glfw/src/x11_window.c"
#include "../External/glfw/src/glx_context.c"
#endif

#include "../External/glfw/src/linux_joystick.c"
#include "../External/glfw/src/posix_thread.c"
#include "../External/glfw/src/posix_time.c"
#include "../External/glfw/src/xkb_unicode.c"
#include "../External/glfw/src/egl_context.c"
#include "../External/glfw/src/osmesa_context.c"
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined( __NetBSD__) || defined(__DragonFly__)
#include "../External/glfw/src/x11_init.c"
#include "../External/glfw/src/x11_monitor.c"
#include "../External/glfw/src/x11_window.c"
#include "../External/glfw/src/xkb_unicode.c"
// TODO: Joystick implementation
#include "../External/glfw/src/null_joystick.c"
#include "../External/glfw/src/posix_time.c"
#include "../External/glfw/src/posix_thread.c"
#include "../External/glfw/src/glx_context.c"
#include "../External/glfw/src/egl_context.c"
#include "../External/glfw/src/osmesa_context.c"
#endif

#if defined(__APPLE__)
#include "../External/glfw/src/cocoa_init.m"
#include "../External/glfw/src/cocoa_joystick.m"
#include "../External/glfw/src/cocoa_monitor.m"
#include "../External/glfw/src/cocoa_window.m"
#include "../External/glfw/src/cocoa_time.c"
#include "../External/glfw/src/posix_thread.c"
#include "../External/glfw/src/nsgl_context.m"
#include "../External/glfw/src/egl_context.c"
#include "../External/glfw/src/osmesa_context.c"
#endif