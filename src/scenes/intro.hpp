#pragma once

#include <bgfx/bgfx.h>
#include "engine/engine.hpp"
#include "engine/iscene.hpp"
#include "scenes/mage.hpp"

class IntroScene : public IScene {
public:

private:
	const uint64_t m_maxticks = 16 * 60 * 5;
	virtual bool onCreate() {
		
		return true;
	}

	virtual void onDestroy() {

	}

	virtual void onTick() {

	}

	virtual void onUpdate() {
		bgfx::touch(0);
		
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(4, 10, 0x0f, "press to skip...");

		bgfx::frame();
	}

	virtual void onEvent(const SDL_Event& event) {
		switch (event.type) {
		case SDL_FINGERUP:
		case SDL_MOUSEBUTTONUP:
			engine::set_scene(*m_context, engine::new_scene<MageScene>(*m_context));
			break;
		};
	}


};

