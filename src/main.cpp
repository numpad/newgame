#include <stdio.h>
#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#if BX_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

#include "engine/engine.hpp"
#include "engine/iscene.hpp"
#include "scenes/mage.hpp"

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
		// timestep
		static uint64_t _time_ms = 0;
		const uint64_t _dt_ms = 16;
		static uint64_t _currenttime_ms = engine::get_scenetime(*ctx);
		static uint64_t _accumulator = 0;

		const uint64_t newtime = engine::get_scenetime(*ctx);
		const uint64_t frametime = newtime - _currenttime_ms;
		_currenttime_ms = newtime;

		_accumulator += frametime;

		while (_accumulator >= _dt_ms) {
			ctx->scene->tick();
			_accumulator -= _dt_ms;
			_time_ms += _dt_ms;
		}

		ctx->scene->update();
	}

#if BX_PLATFORM_EMSCRIPTEN
	if (ctx->quit) {
		emscripten_cancel_main_loop();
		engine::destroy(*ctx);
	}
#endif
}

int main() {
	printf(" .~* Hello World *~.\n");
	
	// init
	EngineContext context;
	engine::init(context);

	// scene setup
	engine::set_scene(context, engine::new_scene<MageScene>(context));

	// main loop
#if BX_PLATFORM_EMSCRIPTEN
	emscripten_set_main_loop_arg(main_loop, &context, -1, 1);
#else
	while (!context.quit) {
		main_loop(&context);
	}

	// cleanup
	engine::destroy(context);
#endif

	return 0;
}

