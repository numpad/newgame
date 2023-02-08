#include "engine/engine.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/mutex.h>
#include <bx/thread.h>
#include <bgfx/bgfx.h>
#include "engine/iscene.hpp"

namespace engine {
	void init(EngineContext& context) {

		// init sdl
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) < 0) {
			printf("failed initializing SDL2: %s\n", SDL_GetError());
			return;
		}
		context.window = SDL_CreateWindow("main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

#if !BX_PLATFORM_EMSCRIPTEN
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
		if (!SDL_GetWindowWMInfo(context.window, &wmi)) {
			printf("Failed getting Window WM Info... Exiting...\n");
			return;
		}
		bgfx::renderFrame();
#endif
		
		bgfx::PlatformData pd;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
		pd.ndt = wmi.info.x11.display;
		pd.nwh = (void*)wmi.info.x11.window;
#elif BX_PLATFORM_WINDOWS
		pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_OSX
		pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_EMSCRIPTEN
		pd.nwh = (void*)"#canvas";
#else
#error "BX_PLATFORM not set!"
#endif
		
		int width, height;
		SDL_GetWindowSize(context.window, &width, &height);

		// init bgfx
		bgfx::Init init;
		init.type = bgfx::RendererType::Count;
		init.resolution.width = width;
		init.resolution.height = height;
		init.resolution.reset = BGFX_RESET_VSYNC;
		init.platformData = pd;
		bgfx::init(init);
		bgfx::setDebug(BGFX_DEBUG_TEXT);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xd65d0eff, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
		
	}

	void set_scene(EngineContext& context, IScene* newScene) {
		if (context.scene) {
			context.scene->destroy();
			delete context.scene;
			context.scene = nullptr;
		}

		if (newScene && newScene->create()) {
			context.scene = newScene;
			context.scene->m_context = &context;
		}
	}
}

