#pragma once

#include <SDL2/SDL.h>

class IScene;

struct EngineContext {
	bool quit = false;
	SDL_Window* window = nullptr;
	IScene* scene = nullptr;
};

namespace engine {
	// general engine
	void init(EngineContext& context);
	void destroy(EngineContext& context);

	// scene management
	void set_scene(EngineContext& context, IScene* newScene);

	// utility functions
	
	/** Returns the time since last scene creation in milliseconds. */
	uint64_t get_scenetime(const EngineContext& context);
}

