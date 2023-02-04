#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/mutex.h>
#include <bx/thread.h>

int main() {
	printf(" .~* Hello World *~.\n");

	// init sdl
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

#if !BX_PLATFORM_EMSCRIPTEN
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(window, &wmi)) {
		printf("Failed getting Window WM Info... Exiting...\n");
		return 1;
	}
#endif
	
	bgfx::PlatformData pd;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	pd.ndt = wmi.info.x11.display;
	pd.nwh = (void *)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_WINDOWS
	pd.ndt = nullptr;
	pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_OSX
	pd.ndt = NULL;
	pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_EMSCRIPTEN
	// nothing i guess?
	// pd.nwh = (void*)"#canvas";
#endif

	// init bgfx
	bgfx::Init init;
	init.type = bgfx::RendererType::Count;
	init.resolution.width = 800;
	init.resolution.height = 600;
	init.resolution.reset = BGFX_RESET_VSYNC;
	init.platformData = pd;
	bgfx::init(init);
	bgfx::setDebug(BGFX_DEBUG_TEXT);
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
	
	// main loop
	for (int i = 0; i < 5000; ++i) {
		bgfx::setViewRect(0, 0, 0, 800, 600);
		bgfx::touch(0);
		bgfx::frame();
	}
	
	// cleanup
	bgfx::shutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();
	printf(" .~*  Bye World  *~.\n");

	return 0;
}

