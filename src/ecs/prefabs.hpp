#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace prefabs {
	
	entt::entity player(entt::registry& registry);

	entt::entity base_enemy(entt::registry& registry, glm::vec2 pos);

	entt::entity item_mana(entt::registry& registry);

	entt::entity attack_swing(entt::registry& registry, glm::vec2 pos, glm::vec2 dir);
}

