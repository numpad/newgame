#pragma once

#include <SDL2/SDL.h>

class IScene;

namespace engine {
	struct Context {
		bool quit = false;
		SDL_Window* window = nullptr;
		IScene* scene = nullptr;
	};
	
	// general engine
	void init(Context& context);
	void destroy(Context& context);

	// scene management
	template <class T>
	IScene* new_scene(const Context& context) { return new T(); }
	void set_scene(Context& context, IScene* newScene);

	// utility functions
	
	/** Returns the time since last scene creation in milliseconds. */
	uint64_t get_scenetime(const Context& context);
}

