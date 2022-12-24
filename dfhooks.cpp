#include "dfhooks.h"

#include <string>

#if _WIN32
#   include <windows.h>
#else
#   include <dlfcn.h>
#   if __APPLE__
        const std::string lib_ext = ".dylib";
#   else
        const std::string lib_ext = ".so";
#   endif
#endif

const std::string g_lib_name = "dfhack";
void *g_lib_handle = nullptr;

typedef void (*dfhack_init_fn)();
dfhack_init_fn g_dfhack_init = nullptr;

typedef void (*dfhack_shutdown_fn)();
dfhack_shutdown_fn g_dfhack_shutdown = nullptr;

typedef void (*dfhack_update_fn)();
dfhack_update_fn g_dfhack_update = nullptr;

typedef void (*dfhack_prerender_fn)();
dfhack_prerender_fn g_dfhack_prerender = nullptr;

typedef bool (*dfhack_sdl_event_fn)(SDL::Event* event);
dfhack_sdl_event_fn g_dfhack_sdl_event = nullptr;

static void * open_library(const std::string &lib_name) {
#if _WIN32
    return LoadLibrary((lib_name + ".dll").c_str());
#else
    return dlopen(("hack/lib" + lib_name + lib_ext).c_str(), RTLD_LAZY);
#endif
}

static void close_lib(void *handle) {
    if (!handle) {
        return;
    }
#if _WIN32
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif
}

static void *load_sym(void *handle, const char *sym) {
    if (!handle) {
        return nullptr;
    }
#if _WIN32
    return (void *)GetProcAddress((HMODULE)handle, sym);
#else
    return dlsym(handle, sym);
#endif
}

static void init_fns(void *handle) {
    g_dfhack_init = (dfhack_init_fn)load_sym(handle, "dfhack_init");
    g_dfhack_shutdown = (dfhack_shutdown_fn)load_sym(handle, "dfhack_shutdown");
    g_dfhack_update = (dfhack_update_fn)load_sym(handle, "dfhack_update");
    g_dfhack_prerender = (dfhack_prerender_fn)load_sym(handle, "dfhack_prerender");
    g_dfhack_sdl_event = (dfhack_sdl_event_fn)load_sym(handle, "dfhack_sdl_event");
}

void hooks_init() {
    g_lib_handle = open_library(g_lib_name);
    init_fns(g_lib_handle);

    if (g_dfhack_init)
        g_dfhack_init();
}

void hooks_shutdown() {
    if (g_dfhack_shutdown)
        g_dfhack_shutdown();

    init_fns(nullptr);
    close_lib(g_lib_handle);
    g_lib_handle = nullptr;
}

void hooks_update() {
    if (g_dfhack_update)
        g_dfhack_update();
}

void hooks_prerender() {
    if (g_dfhack_prerender)
        g_dfhack_prerender();
}

bool hooks_sdl_event(SDL::Event* event) {
    if (g_dfhack_sdl_event)
        return g_dfhack_sdl_event(event);
    return false;
}
