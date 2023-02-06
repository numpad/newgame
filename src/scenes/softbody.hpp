#pragma once

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
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

class SoftBodyScene : public IScene {
public:

	SoftBodyScene(EngineContext* context) : IScene(context) {
	}


private:
	bgfx::VertexBufferHandle m_vbh;
	bgfx::ProgramHandle m_program;

	virtual bool onCreate() {
		Vertex_PosColor::init();
		
		m_vbh = bgfx::createVertexBuffer(
					bgfx::makeRef(s_carVertices, sizeof(s_carVertices)),
					Vertex_PosColor::ms_layout);
		
		m_program = assets::load_program("res/shader/jelly");

		return true;
	}

	virtual void onUpdate() {
		bgfx::touch(0);
		
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(1, 1, 0x0f, "ich bin kein renderer, ich putze hier nur.");
		
		// render
		const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
		const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};

		// view & projection matrix
		float view[16];
		bx::mtxLookAt(view, eye, at);
		float proj[16];
		bx::mtxProj(proj, 60.0f, float(800)/float(600), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		bgfx::setState(BGFX_STATE_WRITE_R
		             | BGFX_STATE_WRITE_G
		             | BGFX_STATE_WRITE_B
		             | BGFX_STATE_WRITE_A); // and more

		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::submit(0, m_program);

		bgfx::frame();
	}

	virtual void onDestroy() {
		bgfx::destroy(m_program);
	}

};

