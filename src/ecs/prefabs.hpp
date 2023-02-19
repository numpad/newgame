#pragma once

#include <entt/entt.hpp>

namespace prefabs {
	
	entt::entity player(entt::registry& registry);

	entt::entity base_enemy(entt::registry& registry);

	entt::entity item_mana(entt::registry& registry);
}

