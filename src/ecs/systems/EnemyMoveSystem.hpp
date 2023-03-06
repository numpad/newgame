#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
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

					pos.pos += glm::normalize(dir) * movetowards->speed;
				}
			}
			
			// keep enemies separated
			glm::vec2 sum = glm::vec2(0.0f);
			m_registry.view<Position, const Enemy>().each([&](const entt::entity otherE, Position& otherPos, const Enemy& otherEnemy) {
				if (e == otherE) return;
				
				const float neighborDist = 36.0f;
				const glm::vec2 otherToMe = otherPos.pos - pos.pos;
				const float dist2 = glm::length2(otherToMe);
				if (dist2 < glm::pow(neighborDist, 2.0f)) {
					const float dist = glm::sqrt(dist2);
					sum += (1.0f / neighborDist) * otherToMe * -0.5f;
				}
			});
			pos.pos += sum;
		});
	}

	void update() const {

	}
};

