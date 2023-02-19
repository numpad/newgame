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
#include <string>

namespace engine {
	void init(Context& context) {

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
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x333333ff, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, width, height);
		
	}

	void update(Context& context) {
		// handle events
		for (SDL_Event event; SDL_PollEvent(&event) != 0; ) {
			
			bool skipEvent = false;

			switch (event.type) {
			case SDL_QUIT:
				context.quit = true;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					const int width = event.window.data1;
					const int height = event.window.data2;
					printf("resize to %dx%d\n", width, height);
					bgfx::reset(width, height, BGFX_RESET_VSYNC);
					bgfx::setViewRect(0, 0, 0, width, height);
				}
				break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				if (event.motion.which == SDL_TOUCH_MOUSEID) {
					skipEvent = true;
					break;
				}
				break;
			}

			// trigger scene event
			if (context.scene != nullptr && !skipEvent) context.scene->event(event);
		}
		
		// render
		if (context.scene != nullptr) {
			// timestep
			static uint64_t _time_ms = 0;
			const uint64_t _dt_ms = 16;
			static uint64_t _currenttime_ms = engine::get_scenetime(context);
			static uint64_t _accumulator = 0;

			const uint64_t newtime = engine::get_scenetime(context);
			const uint64_t frametime = newtime - _currenttime_ms;
			_currenttime_ms = newtime;

			_accumulator += frametime;

			while (_accumulator >= _dt_ms) {
				context.scene->tick();
				_accumulator -= _dt_ms;
				_time_ms += _dt_ms;
			}

			context.scene->update();
		}
	}

	void destroy(Context& context) {
		set_scene(context, nullptr);

		bgfx::shutdown();
		SDL_DestroyWindow(context.window);
		SDL_Quit();
		printf(" .~*  Bye World  *~.\n");
	}

	void set_scene(Context& context, IScene* newScene) {
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

	uint64_t get_scenetime(const Context& context) {
		if (!context.scene->get_createdtime()) return 0;
	
		return SDL_GetTicks64() - context.scene->get_createdtime();
	}
}

