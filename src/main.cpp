#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/mutex.h>
#include <bx/thread.h>
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#if BX_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

#include "engine/engine.hpp"
#include "engine/iscene.hpp"
#include "scenes/softbody.hpp"

void main_onexit(void* data) {
	EngineContext* ctx = static_cast<EngineContext*>(data);

	bgfx::shutdown();
	SDL_DestroyWindow(ctx->window);
	SDL_Quit();
	printf(" .~*  Bye World  *~.\n");
}

void main_loop(void* data) {
	EngineContext* ctx = static_cast<EngineContext*>(data);

	// handle events
	for (SDL_Event event; SDL_PollEvent(&event) != 0; ) {
		switch (event.type) {
		case SDL_QUIT:
			ctx->quit = true;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				const int width = event.window.data1;
				const int height = event.window.data2;
				bgfx::reset(width, height, BGFX_RESET_VSYNC);
				bgfx::setViewRect(0, 0, 0, width, height);
			}
			break;
		case SDL_MOUSEMOTION:
			if (event.motion.which == SDL_TOUCH_MOUSEID) break;

			printf("mouse move\n");
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.which == SDL_TOUCH_MOUSEID) break;

			printf("mouse down\n");
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.which == SDL_TOUCH_MOUSEID) break;

			printf("mouse up\n");
			break;
		case SDL_FINGERMOTION:
			printf("finger move\n");
			break;
		case SDL_FINGERDOWN:
			printf("finger down\n");
			break;
		case SDL_FINGERUP:
			printf("finger up\n");
			break;
		}
	}
	
	// render
	if (ctx->scene != nullptr) {
		ctx->scene->update();
	} else {
		printf("main_loop: scene is NULL!\n");
	}

#if BX_PLATFORM_EMSCRIPTEN
	if (ctx->quit) {
		emscripten_cancel_main_loop();
		main_onexit(data);
	}
#endif
}

int main() {
	printf(" .~* Hello World *~.\n");
	
	// init
	EngineContext context;

	// init sdl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) < 0) {
		printf("failed initializing SDL2: %s\n", SDL_GetError());
		return 1;
	}
	context.window = SDL_CreateWindow("main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

#if !BX_PLATFORM_EMSCRIPTEN
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(context.window, &wmi)) {
		printf("Failed getting Window WM Info... Exiting...\n");
		return 1;
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
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xd65d0eff, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
	
	// scene setup
	context.scene = new SoftBodyScene(&context);
	context.scene->create();

	// main loop
#if BX_PLATFORM_EMSCRIPTEN
	emscripten_set_main_loop_arg(main_loop, &context, -1, 1);
#else
	while (!context.quit) {
		main_loop(&context);
	}

	// cleanup
	main_onexit(&context);
#endif

	return 0;
}

