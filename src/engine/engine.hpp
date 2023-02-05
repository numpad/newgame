#pragma once

#include <SDL2/SDL.h>

class IScene;

struct EngineContext {
	bool quit = false;
	SDL_Window* window = nullptr;
	IScene* scene = nullptr;
};

class Engine {
public:

	void set_scene(IScene* scene);

private:
	
	bool init();
};

