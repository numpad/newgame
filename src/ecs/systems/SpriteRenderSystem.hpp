#pragma once

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "engine/assets.hpp"
#include "ecs/components/position.hpp"
#include "ecs/components/sprite.hpp"

struct VertexPosSubrectColor {
	float m_x, m_y, m_z;
	float m_u, m_v;
	uint32_t m_abgr;

	static void init() {
		ms_layout.begin()
			.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
			.end();
	}

	static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout VertexPosSubrectColor::ms_layout;

static VertexPosSubrectColor s_vertices[] = {
	{-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  0xff0000ff},
	{ 1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  0xff0000ff},
	{-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,  0xffff0000},
	{-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,  0xffff0000},
	{ 1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  0xff0000ff},
	{ 1.0f,  1.0f, 0.0f,  0.0f, 0.0f,  0xffff0000}
};

class SpriteRenderSystem {
public:
	
	SpriteRenderSystem(entt::registry& registry) : m_registry{registry} {
		VertexPosSubrectColor::init();

		m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_vertices, sizeof(s_vertices)), VertexPosSubrectColor::ms_layout);
		m_program = assets::load_program("res/shader/sprite");

	}

	~SpriteRenderSystem() {
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_program);
	}

	void render() const {
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::setState(BGFX_STATE_WRITE_RGB);

		m_registry.view<const Position, const Sprite>().each([&](const Position& pos, const Sprite& sprite) {
			glm::mat4 transform = glm::scale(
				glm::translate(glm::mat4(1.0f),
					glm::vec3(pos.pos, 0.0f)),
				glm::vec3(0.2f, 0.2f, 0.0f)
			);
			
			bgfx::setTransform(&transform);
			bgfx::submit(0, m_program);
		});
	}

private:
	entt::registry& m_registry;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::ProgramHandle m_program;

};

