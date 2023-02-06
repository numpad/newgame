#pragma once

#include <SDL2/SDL.h>

class IScene;

struct EngineContext {
	bool quit = false;
	SDL_Window* window = nullptr;
	IScene* scene = nullptr;
};

namespace engine {
	void set_scene(EngineContext& context, IScene* newScene);
}

