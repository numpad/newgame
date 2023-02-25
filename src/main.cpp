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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#include "engine/engine.hpp"
#include "engine/iscene.hpp"
#include "scenes/intro.hpp"

void main_loop(void* data) {
	engine::Context* ctx = static_cast<engine::Context*>(data);

	engine::update(*ctx);

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
	engine::Context context;
	engine::init(context);

	// scene setup
	engine::set_scene(context, engine::new_scene<IntroScene>(context));

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

