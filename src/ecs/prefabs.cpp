#include "ecs/prefabs.hpp"

#include <entt/entt.hpp>
#include "ecs/components/position.hpp"
#include "ecs/components/sprite.hpp"
#include "ecs/components/player.hpp"
#include "ecs/components/enemy.hpp"
#include "ecs/components/damage_zone.hpp"
#include "ecs/components/mana_item.hpp"

namespace prefabs {
	
	entt::entity player(entt::registry& registry) {
		entt::entity e = registry.create();
		registry.emplace<Position>(e, glm::vec2(0.0f, 0.0f));
		registry.emplace<Sprite>(e, glm::vec2(32.0f), glm::vec4(0.0f));
		registry.emplace<Player>(e, 28.0f, 0.0f, 5.0f);
		return e;
	}

	entt::entity base_enemy(entt::registry& registry) {
		entt::entity e = registry.create();
		registry.emplace<Position>(e, glm::vec2(0.0f, 500.0f));
		registry.emplace<Enemy>(e, 20, 20);
		registry.emplace<Sprite>(e, glm::vec2(24.0f), glm::vec4(0.0f));
		registry.emplace<DamageZone>(e, 2.0f, 15.0f, DamageLayer::PLAYER);
		return e;
	}

	entt::entity item_mana(entt::registry& registry) {
		entt::entity e = registry.create();
		registry.emplace<Position>(e, glm::vec2(100.0f, -300.0f));
		registry.emplace<ManaItem>(e, 1.0f, -1, glm::vec2());
		registry.emplace<Sprite>(e, glm::vec2(12.0f), glm::vec4(0.0f));
		return e;
	}
}

