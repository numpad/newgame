#pragma once

#include <bx/easing.h>
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

	static void init() {
		ms_layoutSprite.begin()
			.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	}

	static bgfx::VertexLayout ms_layoutSprite;
};
bgfx::VertexLayout VertexPosSubrectColor::ms_layoutSprite;

static VertexPosSubrectColor s_vertices[] = {
	/* position           texcoord  */
	{-1.0f,  1.0f, 0.0f,  0.0f, 1.0f },
	{ 1.0f,  1.0f, 0.0f,  1.0f, 1.0f },
	{-1.0f, -1.0f, 0.0f,  0.0f, 0.0f },
	{-1.0f, -1.0f, 0.0f,  0.0f, 0.0f },
	{ 1.0f,  1.0f, 0.0f,  1.0f, 1.0f },
	{ 1.0f, -1.0f, 0.0f,  1.0f, 0.0f }
};

class SpriteRenderSystem {
private:
	entt::registry& m_registry;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::ProgramHandle m_program;
	bgfx::TextureHandle m_texture;
	bgfx::UniformHandle m_utexture;
	bgfx::UniformHandle m_utexcoords;
	bgfx::UniformHandle m_udamageflash;

public:
	
	SpriteRenderSystem(entt::registry& registry) : m_registry{registry} {
		VertexPosSubrectColor::init();

		m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_vertices, sizeof(s_vertices)), VertexPosSubrectColor::ms_layoutSprite);
		m_program = assets::load_program("res/shader/sprite");
		m_texture = assets::load_texture("res/image/dungeon.png");
		m_utexture = bgfx::createUniform("u_texture", bgfx::UniformType::Sampler);
		m_utexcoords = bgfx::createUniform("u_texcoords", bgfx::UniformType::Vec4);
		m_udamageflash = bgfx::createUniform("u_damageflash", bgfx::UniformType::Vec4);
	}

	~SpriteRenderSystem() {
		bgfx::destroy(m_texture);
		bgfx::destroy(m_utexture);
		bgfx::destroy(m_utexcoords);
		bgfx::destroy(m_udamageflash);
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_program);
	}

	void tick() {
		constexpr float dt = 1.0f / 60.0f;

		m_registry.view<Sprite>().each([&](Sprite& sprite) {
			sprite.damageflash.x -= dt * 5.0f;
			if (sprite.damageflash.x < 0.0f) {
				sprite.damageflash.x = 0.0f;
			}
		});
	}

	void render() const {

		m_registry.view<const Position, const Sprite>().each([&](const Position& pos, const Sprite& sprite) {
			glm::mat4 transform = glm::translate(glm::scale(
				glm::rotate(
					glm::translate(glm::mat4(1.0f), glm::vec3(pos.pos, 0.0f)),
					sprite.angle, glm::vec3(0.0f, 0.0f, 1.0f)
				),
				glm::vec3(sprite.size.x, sprite.size.y, 0.0f)
			), glm::vec3(sprite.offset, 0.0f));
			
			// convert `damageflash` to interpolated value for rendering
			glm::vec4 damageflash = sprite.damageflash;
			damageflash.x = bx::easeOutExpo(sprite.damageflash.x);

			bgfx::setVertexBuffer(0, m_vbh);
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_ALPHA);
			bgfx::setTransform(&transform);
			bgfx::setUniform(m_utexcoords, &sprite.rect);
			bgfx::setUniform(m_udamageflash, &damageflash);
			bgfx::setTexture(0, m_utexture, m_texture, BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP);
			bgfx::submit(0, m_program);
		});
	}

};

