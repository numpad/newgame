#pragma once

#include <bgfx/bgfx.h>
#include "engine/engine.hpp"
#include "engine/iscene.hpp"

class SimpleScene : public IScene {
public:

private:
	
	virtual bool onCreate() {
		
		return true;
	}

	virtual void onTick() {

	}

	virtual void onUpdate() {
		bgfx::touch(0);
		
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(1, 1, 0x0f, "+- simple scene --------+");
		bgfx::dbgTextPrintf(1, 2, 0x0f, "|                       |");
		bgfx::dbgTextPrintf(1, 3, 0x0f, "| the most basic things |");
		bgfx::dbgTextPrintf(1, 4, 0x0f, "| should work.          |");
		bgfx::dbgTextPrintf(1, 5, 0x0f, "|                       |");
		bgfx::dbgTextPrintf(1, 6, 0x0f, "|     [ cancel ] [ OK ] |");
		bgfx::dbgTextPrintf(1, 7, 0x0f, "+-----------------------+");
		

		bgfx::frame();
	}

	virtual void onDestroy() {

	}

};

