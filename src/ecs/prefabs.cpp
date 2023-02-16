#include "ecs/prefabs.hpp"

#include <entt/entt.hpp>
#include "ecs/components/position.hpp"
#include "ecs/components/sprite.hpp"
#include "ecs/components/player.hpp"

namespace prefabs {
	
	entt::entity player(entt::registry& registry) {
		entt::entity e = registry.create();
		registry.emplace<Position>(e, glm::vec3(0.0f, 0.0f, 0.1f));
		registry.emplace<Sprite>(e, glm::vec2(32.0f), glm::vec4(0.0f));
		registry.emplace<Player>(e, nullptr);
		return e;
	}

	entt::entity base_enemy(entt::registry& registry) {
		entt::entity e = registry.create();
		registry.emplace<Position>(e, glm::vec3(0.0f, 0.0f, 0.1f));
		registry.emplace<Sprite>(e, glm::vec2(0.05f), glm::vec4(0.0f));
		return e;
	}
}

