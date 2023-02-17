#pragma once

#include <entt/entt.hpp>
#include "ecs/components/position.hpp"
#include "ecs/components/enemy.hpp"
#include "ecs/components/move_towards.hpp"
#include "ecs/components/move_direction.hpp"

class EnemyMoveSystem {
private:
	entt::registry& m_registry;

public:

	EnemyMoveSystem(entt::registry& registry) : m_registry{registry} {

	}

	void tick() const {
		m_registry.view<Position, const Enemy>().each([&](const entt::entity e, Position& pos, const Enemy& enemy) {
			const MoveTowards* movetowards = m_registry.try_get<MoveTowards>(e);
			const MoveDirection* movedirection = m_registry.try_get<MoveDirection>(e);

			if (movetowards) {
				const Position* target_pos = m_registry.try_get<Position>(movetowards->target);
				
				if (target_pos) {
					const glm::vec2 dir = (target_pos->pos - pos.pos);

					pos.pos += glm::normalize(dir) * 2.0f;
				}
			}
		});
	}

	void update() const {

	}
};

