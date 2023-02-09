#pragma once

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/engine.hpp"
#include "engine/iscene.hpp"
#include "engine/assets.hpp"

struct Vertex_PosColor {
	float m_x, m_y, m_z;
	uint32_t m_abgr;

	static void init() {
		ms_layout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
			.end();
	}

	static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout Vertex_PosColor::ms_layout;

static Vertex_PosColor s_carVertices[] = {
	{-1.0f, -1.0f, 0.0f, 0xff0000ff},
	{ 1.0f, -1.0f, 0.0f, 0xff00ff00},
	{-1.0f,  1.0f, 0.0f, 0xffff0000},
	{-1.0f,  1.0f, 0.0f, 0xffff0000},
	{ 1.0f, -1.0f, 0.0f, 0xff00ff00},
	{ 1.0f,  1.0f, 0.0f, 0xff00ffff}
};

class MageScene : public IScene {
public:

private:
	bgfx::VertexBufferHandle m_vbh;
	bgfx::ProgramHandle m_program;
	uint64_t timeaccu = 0;

	virtual bool onCreate() {
		Vertex_PosColor::init();
		
		m_vbh = bgfx::createVertexBuffer(
					bgfx::makeRef(s_carVertices, sizeof(s_carVertices)),
					Vertex_PosColor::ms_layout);
		
		m_program = assets::load_program("res/shader/jelly");

		return true;
	}
	
	virtual void onTick() {
		timeaccu += 16;
	}

	virtual void onUpdate() {
		bgfx::touch(0);
		
		float time = engine::get_scenetime(*m_context) / 1000.0f;
		float time2 = timeaccu / 1000.0f;
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(1, 1, 0x0f, "seconds: %g", time);
		bgfx::dbgTextPrintf(1, 2, 0x0f, "seconds: %g", time2);
		bgfx::dbgTextPrintf(1, 3, 0x0f, "   diff: %g", time - time2);
		
		// render
		int width, height;
		SDL_GetWindowSize(m_context->window, &width, &height);
		const float aspect = float(width) / float(height);

		glm::mat4 proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		bgfx::setViewTransform(0, &view, &proj);
		
		glm::mat4 model = glm::mat4(1.0f);
		bgfx::setTransform(&model);

		bgfx::setState(BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G
		             | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A); // and more?

		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::submit(0, m_program);

		bgfx::frame();
	}

	virtual void onDestroy() {
		bgfx::destroy(m_program);
		bgfx::destroy(m_program);
	}

};

