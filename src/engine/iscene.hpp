#pragma once

#include <SDL2/SDL.h>
#include "engine/engine.hpp"

class IScene {
public:
	
	EngineContext* m_context = nullptr;

	virtual ~IScene() = default;

	bool create() {
		m_createdtime = SDL_GetTicks64();
		return onCreate();
	}
	void tick() { onTick(); }
	void update() { onUpdate(); }
	void destroy() { onDestroy(); }

	uint64_t get_createdtime() const { return m_createdtime; }

private:
	uint64_t m_createdtime;

	virtual bool onCreate() = 0;
	virtual void onTick() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;

};

