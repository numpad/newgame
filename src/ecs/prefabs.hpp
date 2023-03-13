#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace prefabs {
	
	// entities
	entt::entity player(entt::registry& registry);
	entt::entity base_enemy(entt::registry& registry, glm::vec2 pos, entt::entity target);

	// collectable items
	entt::entity item_mana(entt::registry& registry);

	// attacks
	entt::entity attack_swing(entt::registry& registry, glm::vec2 pos, glm::vec2 dir);
	entt::entity attack_lavazone(entt::registry& registry, glm::vec2 pos);
}

