#pragma once

#include <SDL2/SDL.h>

class IScene;

namespace engine {
	struct Timestep {
		float time;
		float dt;
		uint64_t currenttime;
		float accumulator;

		uint32_t remaining_ticks;
	};

	struct Context;
	void timestep_init(const Context& context, Timestep& timestep);
	void timestep_advance(const Context& context, Timestep& timestep);

	struct Context {
		bool quit = false;
		SDL_Window* window = nullptr;
		IScene* scene = nullptr;
		IScene* next_scene = nullptr;
		Timestep timestep;
	};
	
	// general engine
	void init(Context& context);
	void destroy(Context& context);

	void update(Context& context);

	// scene management
	template <class T>
	IScene* new_scene(const Context& context) { return new T(); }
	void set_scene(Context& context, IScene* newScene);

	// utility functions
	
	/** Returns the time since last scene creation in milliseconds. */
	uint64_t get_scenetime(const Context& context);
}

