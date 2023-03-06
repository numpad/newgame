#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <entt/entt.hpp>
#include "ecs/components/position.hpp"
#include "ecs/components/sprite.hpp"
#include "ecs/components/player.hpp"
#include "ecs/components/enemy.hpp"
#include "ecs/components/damage_zone.hpp"
#include "ecs/components/deletable.hpp"

class DamageSystem {
private:
	entt::registry& m_registry;

public:

	DamageSystem(entt::registry& registry) : m_registry{registry} {

	}

	void tick() const {
		auto damage_zones = m_registry.view<const Position, const DamageZone>();
		// TODO: find a better way to identify 
		auto damage_targets = m_registry.view<const Position, const Sprite>();

		damage_zones.each([&](const entt::entity zoneEntity, const Position& zonePos, const DamageZone& zone) {

			for (auto targetEntity: damage_targets) {
				Position targetPos = m_registry.get<Position>(targetEntity);
				Player* player = m_registry.try_get<Player>(targetEntity);
				Enemy* enemy = m_registry.try_get<Enemy>(targetEntity);

				const float distance2 = glm::distance2(targetPos.pos, zonePos.pos);
				if (player && (zone.layer & DamageLayer::PLAYER)) {
					if (distance2 <= glm::pow(zone.radius, 2.0f)) {
						// printf("player: ouch\n");
					}
				}

				if (enemy && (zone.layer & DamageLayer::ENEMY)) {
					if (distance2 <= glm::pow(zone.radius, 2.0f)) {
						printf("enemy: %g/%g\n", enemy->hp, enemy->max_hp);
						enemy->hp -= zone.dps * (1.0f / 60.0f); // FIXME: engine timestep FPS

						if (enemy->hp <= 0.0f) {
							enemy->hp = 0.0f;
							m_registry.emplace_or_replace<Deletable>(targetEntity);
						}
					}
				}
			}
		});
	}

	void update() const {

	}
};

