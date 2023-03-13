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
	
	void timestep_init(const Context& context, Timestep& timestep) {
		timestep.time = 0.0f;
		timestep.dt = 1.0f / 60.0f;
		timestep.currenttime = engine::get_scenetime(context);
		timestep.accumulator = 0.0f;
		timestep.remaining_ticks = 0;
	}

	void timestep_advance(const Context& context, Timestep& timestep) {
		const uint64_t newtime = engine::get_scenetime(context);
		const uint64_t frametime = newtime - timestep.currenttime;
		timestep.currenttime = newtime;

		timestep.accumulator += frametime / 1000.0f;
		
		while (timestep.accumulator >= timestep.dt) {
			timestep.remaining_ticks++;
			timestep.accumulator -= timestep.dt;
			timestep.time += timestep.dt;
		}
	}

	void init(Context& context) {

		// init sdl
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) < 0) {
			printf("failed initializing SDL2: %s\n", SDL_GetError());
			return;
		}
		context.window = SDL_CreateWindow("main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 490, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x333333ff, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, width, height);
		
		// init timestep
		engine::timestep_init(context, context.timestep);
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
					bgfx::reset(width, height, BGFX_RESET_VSYNC);
					bgfx::setViewRect(0, 0, 0, width, height);
				} else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
					
				} else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
					
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
		bgfx::touch(0);
		if (context.scene) {
			engine::timestep_advance(context, context.timestep);

			while (context.timestep.remaining_ticks > 0) {
				context.timestep.remaining_ticks--;
				context.scene->tick();
			}

			context.scene->update();
		}

		// switch to next scene
		if (context.next_scene) {
			// destroy current scene
			if (context.scene) {
				context.scene->destroy();
				delete context.scene;
				context.scene = nullptr;
			}
			
			context.scene = context.next_scene;
			context.next_scene = nullptr;

			// reset timestep
			engine::timestep_init(context, context.timestep);
		}

		bgfx::frame(0);
	}

	void destroy(Context& context) {
		// cleanup scene
		set_scene(context, nullptr);
		
		// cleanup engine
		bgfx::shutdown();
		SDL_DestroyWindow(context.window);
		SDL_Quit();
		printf(" .~*  Bye World  *~.\n");
	}

	void set_scene(Context& context, IScene* newScene) {
		if (newScene && newScene->create()) {
			context.next_scene = newScene;
			context.next_scene->m_context = &context;
		}
	}

	uint64_t get_scenetime(const Context& context) {
		if (!context.scene || !context.scene->get_createdtime()) return 0;
	
		return SDL_GetTicks64() - context.scene->get_createdtime();
	}
}

