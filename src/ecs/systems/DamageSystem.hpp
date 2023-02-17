#pragma once

#include <entt/entt.hpp>
#include "ecs/components/position.hpp"
#include "ecs/components/sprite.hpp"
#include "ecs/components/player.hpp"
#include "ecs/components/enemy.hpp"
#include "ecs/components/damage_zone.hpp"

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


				if (player && (zone.layer & DamageLayer::PLAYER)) {
					if (glm::length(targetPos.pos - zonePos.pos) <= zone.radius) {
						printf("player: ouch\n");
					}
				}

				if (enemy && (zone.layer & DamageLayer::ENEMY)) {
					if (glm::length(targetPos.pos - zonePos.pos) <= zone.radius) {
						printf("enemy: oof\n");
					}
				}
			}
		});
	}

	void update() const {

	}
};

