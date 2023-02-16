#pragma once

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	{-1.0f,  1.0f, 0.0f,  0.0625f * 1.0f, 0.0625f * 6.0f,  0xff0000ff},
	{ 1.0f,  1.0f, 0.0f,  0.0625f * 2.0f, 0.0625f * 6.0f,  0xff0000ff},
	{-1.0f, -1.0f, 0.0f,  0.0625f * 1.0f, 0.0625f * 5.0f,  0xffff0000},
	{-1.0f, -1.0f, 0.0f,  0.0625f * 1.0f, 0.0625f * 5.0f,  0xffff0000},
	{ 1.0f,  1.0f, 0.0f,  0.0625f * 2.0f, 0.0625f * 6.0f,  0xff0000ff},
	{ 1.0f, -1.0f, 0.0f,  0.0625f * 2.0f, 0.0625f * 5.0f,  0xffff0000}
};

class SpriteRenderSystem {
public:
	
	SpriteRenderSystem(entt::registry& registry) : m_registry{registry} {
		VertexPosSubrectColor::init();

		m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_vertices, sizeof(s_vertices)), VertexPosSubrectColor::ms_layout);
		m_program = assets::load_program("res/shader/sprite");
		m_texture = assets::load_texture("res/image/dungeon.png");
		m_utexture = bgfx::createUniform("u_texture", bgfx::UniformType::Sampler);
		
	}

	~SpriteRenderSystem() {
		bgfx::destroy(m_texture);
		bgfx::destroy(m_utexture);
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_program);
	}

	void render() const {

		m_registry.view<const Position, const Sprite>().each([&](const Position& pos, const Sprite& sprite) {
			glm::mat4 transform = glm::scale(
				glm::translate(glm::mat4(1.0f), glm::vec3(pos.pos, 0.0f)),
				glm::vec3(sprite.size.x, sprite.size.y, 0.0f)
			);
			
			bgfx::setVertexBuffer(0, m_vbh);
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_ALPHA);
			bgfx::setTransform(&transform);
			bgfx::setTexture(0, m_utexture, m_texture,
				BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP);
			bgfx::submit(0, m_program);
		});
	}

private:
	entt::registry& m_registry;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::ProgramHandle m_program;
	bgfx::TextureHandle m_texture;
	bgfx::UniformHandle m_utexture;

};

