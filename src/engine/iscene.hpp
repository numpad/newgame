#pragma once

#include "engine/engine.hpp"

class IScene {
public:
	
	EngineContext* m_context = nullptr;

	IScene(EngineContext* context) {
		m_context = context;
	}

	virtual ~IScene() = default;

	bool create() { return onCreate(); }
	void update() { onUpdate(); }
	void destroy() { onDestroy(); }

private:
	virtual bool onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;

};

