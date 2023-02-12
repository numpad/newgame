#pragma once

#include <SDL2/SDL.h>
#include "engine/engine.hpp"

class IScene {
public:
	
	engine::Context* m_context = nullptr;

	virtual ~IScene() = default;
	
	bool create() {
		m_createdtime = SDL_GetTicks64();
		return onCreate();
	}
	void destroy() { onDestroy(); }

	void event(const SDL_Event& event) { onEvent(event); }
	// Called with a fixed timestep, usually at 16ms per tick.
	void tick() { onTick(); }

	// Called every frame
	void update() { onUpdate(); }

	uint64_t get_createdtime() const { return m_createdtime; }

private:
	uint64_t m_createdtime;

	virtual bool onCreate() = 0;
	virtual void onDestroy() = 0;
	virtual void onEvent(const SDL_Event& event) {}
	virtual void onTick() = 0;
	virtual void onUpdate() = 0;

};

