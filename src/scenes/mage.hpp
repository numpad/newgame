#pragma once

#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entt/entt.hpp>
#include "engine/engine.hpp"
#include "engine/iscene.hpp"
#include "engine/assets.hpp"
#include "ecs/systems/SpriteRenderSystem.hpp"
#include "ecs/systems/EnemyMoveSystem.hpp"
#include "ecs/systems/DamageSystem.hpp"
#include "ecs/systems/ItemCollectSystem.hpp"
#include "ecs/systems/DeleteSystem.hpp"
#include "ecs/prefabs.hpp"
#include "ecs/components/velocity.hpp"
#include "ecs/components/player.hpp"

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

static Vertex_PosColor s_bgVertices[] = {
	{-1.0f, -1.0f, -1.0f, 0xff0000ff},
	{ 1.0f, -1.0f, -1.0f, 0xff00ff00},
	{-1.0f,  1.0f, -1.0f, 0xffff0000},
	{-1.0f,  1.0f, -1.0f, 0xffff0000},
	{ 1.0f, -1.0f, -1.0f, 0xff00ff00},
	{ 1.0f,  1.0f, -1.0f, 0xff00ffff}
};

class MageScene : public IScene {
	// background
	bgfx::VertexBufferHandle m_vbh;
	bgfx::ProgramHandle m_program;
	bgfx::TextureHandle m_bgTexture;
	bgfx::UniformHandle m_bgTextureU;
	bgfx::UniformHandle m_bgOffsetU;

	// ecs
	entt::registry m_registry;
	SpriteRenderSystem* m_spriterenderer;
	EnemyMoveSystem* m_enemymover;
	DamageSystem* m_damagesystem;
	ItemCollectSystem* m_itemcollectsystem;
	DeleteSystem* m_deletesystem;

	entt::entity m_player = entt::null;
	
	// general
	bool m_dragging = false;
	glm::vec2 m_dragstart, m_dragcurrent;
	uint64_t timeaccu = 0;

	virtual bool onCreate() {
		// init old rendering
		Vertex_PosColor::init();

		m_vbh = bgfx::createVertexBuffer(
					bgfx::makeRef(s_bgVertices, sizeof(s_bgVertices)),
					Vertex_PosColor::ms_layout);
		m_program = assets::load_program("res/shader/jelly");
		m_bgTexture = assets::load_texture("res/image/proto.png");
		m_bgTextureU = bgfx::createUniform("u_texture", bgfx::UniformType::Sampler);
		m_bgOffsetU = bgfx::createUniform("u_offset", bgfx::UniformType::Vec4);

		// init systems
		m_spriterenderer = new SpriteRenderSystem(m_registry);
		m_enemymover = new EnemyMoveSystem(m_registry);
		m_damagesystem = new DamageSystem(m_registry);
		m_itemcollectsystem = new ItemCollectSystem(m_registry);
		m_deletesystem = new DeleteSystem(m_registry);

		// create player
		m_player = prefabs::player(m_registry);

		//entt::entity enemy = prefabs::base_enemy(m_registry);
		//m_registry.emplace<MoveTowards>(enemy, m_player);

		for (int x = 0; x < 5; ++x) {
			for (int y = 0; y < 5; ++y) {
				entt::entity item = prefabs::item_mana(m_registry);
				m_registry.replace<Position>(item, glm::vec2(-80.0f + x * 40.0f, -80.0f + y * 40.0f));
			}
		}

		
		return true;
	}
	
	virtual void onDestroy() {
		delete m_spriterenderer;
		delete m_enemymover;
		delete m_damagesystem;
		delete m_itemcollectsystem;
		delete m_deletesystem;
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_program);
		bgfx::destroy(m_bgTexture);
		bgfx::destroy(m_bgTextureU);
		bgfx::destroy(m_bgOffsetU);
	}
	
	virtual void onEvent(const SDL_Event& event) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN: {
			m_dragging = true;
			m_dragstart = glm::vec2(float(event.button.x), float(event.button.y));
			m_dragcurrent = m_dragstart;
			break;
		}
		case SDL_MOUSEBUTTONUP:
			m_dragging = false;
			break;
		case SDL_MOUSEMOTION:
			m_dragcurrent = glm::vec2(float(event.motion.x), float(event.motion.y));
			break;
		case SDL_FINGERDOWN: {
			int width, height;
			SDL_GetWindowSize(m_context->window, &width, &height);
			
			m_dragging = true;
			m_dragstart = glm::vec2(event.tfinger.x, event.tfinger.y) * glm::vec2(float(width), float(height));
			m_dragcurrent = m_dragstart;
			//int width, height;
			//SDL_GetWindowSize(m_context->window, &width, &height);
			//const float aspect = float(width) / float(height);
			//const float x = (event.tfinger.x * 2.0f - 1.0f) * aspect;
			//const float y = (event.tfinger.y * 2.0f - 1.0f) * -1.0f;
			break;
		}
		case SDL_FINGERUP: {
			m_dragging = false;
			break;
		}
		case SDL_FINGERMOTION: {
			int width, height;
			SDL_GetWindowSize(m_context->window, &width, &height);

			m_dragcurrent = glm::vec2(event.tfinger.x, event.tfinger.y) * glm::vec2(float(width), float(height));
			break;
		}
		};
	}

	virtual void onTick() {
		timeaccu += 16;
		
		const glm::vec2 dragdir = (m_dragcurrent - m_dragstart) * glm::vec2(1.0f, -1.0f);
		if (m_dragging && glm::length(dragdir) > 0.05f) {
			const glm::vec2 drag = glm::normalize(dragdir) * 0.01f;

			// move player
			Position* player_pos = m_registry.try_get<Position>(m_player);
			Player* player = m_registry.try_get<Player>(m_player);
			if (player_pos) {
				player_pos->pos += glm::normalize(dragdir) * player->move_speed;
			}
		}
		
		// tick systems
		m_enemymover->tick();
		m_damagesystem->tick();
		m_itemcollectsystem->tick();
		m_deletesystem->tick();
	}

	virtual void onUpdate() {
		float time = engine::get_scenetime(*m_context) / 1000.0f;
		float time2 = timeaccu / 1000.0f;
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(1, 1, 0x0f, "sys: %g", time);
		bgfx::dbgTextPrintf(1, 2, 0x0f, " dt: %g", time2);

		const Player& player = m_registry.get<const Player>(m_player);
		bgfx::dbgTextPrintf(1, 4, 0x0f, "- Player -");
		bgfx::dbgTextPrintf(1, 5, 0x0f, "Mana: %g", player.mana);
		
		// render
		int width, height;
		SDL_GetWindowSize(m_context->window, &width, &height);
		const float aspect = float(width) / float(height);
		
		// matrices
		const Position* player_pos = m_registry.try_get<Position>(m_player);
		glm::mat4 proj = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-player_pos->pos, 0.0f));
		glm::mat4 model = glm::mat4(1.0f);
		bgfx::setViewTransform(0, &view, &proj);
		bgfx::setTransform(&model);

		// draw background
		const glm::vec2 offset = -/*glm::fract*/(player_pos->pos * 0.5f) * 2.0f;

		bgfx::setUniform(m_bgOffsetU, &offset);
		bgfx::setState(BGFX_STATE_WRITE_RGB); // and more?
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::setTexture(0, m_bgTextureU, m_bgTexture, BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC);
		bgfx::submit(0, m_program);
		
		// systems
		m_spriterenderer->render();
	}
};

