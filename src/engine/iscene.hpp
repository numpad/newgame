#pragma once

#include "engine/engine.hpp"

class IScene {
public:
	
	EngineContext* m_context = nullptr;

	virtual ~IScene() = default;

	bool create() { return onCreate(); }
	void update(float dt) { onUpdate(dt); }
	void destroy() { onDestroy(); }

private:
	virtual bool onCreate() = 0;
	virtual void onUpdate(float dt) = 0;
	virtual void onDestroy() = 0;

};

