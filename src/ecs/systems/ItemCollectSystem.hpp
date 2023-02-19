#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>
#include <entt/entt.hpp>
#include "ecs/components/position.hpp"
#include "ecs/components/player.hpp"
#include "ecs/components/mana_item.hpp"
#include "ecs/components/deletable.hpp"
#include "ecs/prefabs.hpp"

class ItemCollectSystem {
private:
	entt::registry& m_registry;
	uint32_t m_ticksSinceOrbSpawn;

public:

	ItemCollectSystem(entt::registry& registry) : m_registry{registry} {
		m_ticksSinceOrbSpawn = 0;
	}

	void tick() {
		m_ticksSinceOrbSpawn += 16;

		// get player entity
		entt::entity playerEntity = entt::null;
		m_registry.view<const Position, const Player>().each([&playerEntity](const entt::entity entity, const Position& playerPos, const Player& player){
			playerEntity = entity;
		});
		Player& player = m_registry.get<Player>(playerEntity);
		const Position& playerPos = m_registry.get<Position>(playerEntity);

		m_registry.view<Position, ManaItem>().each([&](const entt::entity itemEntity, Position& itemPos, ManaItem& item) {

			// only handle collection
			if (item.animationTicks < 0) {
				const glm::vec2 playerToItem = itemPos.pos - playerPos.pos;
				if (glm::length2(playerToItem) <= glm::pow(player.collect_radius, 2.0f)) {
					item.animationTicks = 0; // start collection logic
					item.animationVel = glm::normalize(playerToItem) * 10.0f;
				}

				return;
			}
			
			// item has been collected
			if (item.animationTicks >= 0) {
				item.animationTicks += 16;

				if (item.animationTicks <= 1100) {
					// collected... move away from player
					itemPos.pos += item.animationVel;
					item.animationVel *= 0.92f;
				} else if (glm::distance2(itemPos.pos, playerPos.pos) > 18.0f) {
					// now move towards player
					const glm::vec2 itemToPlayer = glm::normalize(playerPos.pos - itemPos.pos) * player.move_speed * 2.5f;
					itemPos.pos += itemToPlayer;
				} else {
					// actually collected
					m_registry.emplace_or_replace<Deletable>(itemEntity);
					player.mana += item.mana;
				}
			}

		});

		// spawn orbs
		if (m_ticksSinceOrbSpawn > 850) {
			m_ticksSinceOrbSpawn -= 850;
			
			const entt::entity orb = prefabs::item_mana(m_registry);
			const glm::vec2 dir = glm::circularRand(1.0f);
			const float dist = glm::linearRand(0.9f, 1.1f);
			glm::vec2 p = playerPos.pos + (dir * 300.0f * dist);
			m_registry.replace<Position>(orb, p);
		}
	}

	void update() const {

	}
};


