#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "ecs/components/position.hpp"
#include "ecs/components/player.hpp"
#include "ecs/components/move_towards.hpp"
#include "ecs/prefabs.hpp"

class EnemySpawnSystem {
private:
	entt::registry& m_registry;
	uint32_t m_ticks;
public:
	
	EnemySpawnSystem(entt::registry& registry) : m_registry{registry}, m_ticks{0} {

	}

	void tick() {
		
		
		// find the average player position 
		glm::vec2 average_pos = glm::vec2(0.0f);
		int player_amount = 0;
		entt::entity target_player = entt::null;
		const auto players = m_registry.view<const Position, const Player>();
		players.each([&](const entt::entity e, const Position& pos, const Player& player) {
			average_pos	+= pos.pos;
			++player_amount;
			target_player = e;
		});
		average_pos = average_pos / float(player_amount);

		// spawn enemies
		if (m_ticks % 60 == 0) {
			glm::vec2 offset = glm::diskRand(200.0f);
			prefabs::base_enemy(m_registry, average_pos + offset, target_player);
		}

		++m_ticks;
	}
};

