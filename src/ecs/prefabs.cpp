#include "ecs/prefabs.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <entt/entt.hpp>
#include "ecs/components/position.hpp"
#include "ecs/components/sprite.hpp"
#include "ecs/components/player.hpp"
#include "ecs/components/enemy.hpp"
#include "ecs/components/damage_zone.hpp"
#include "ecs/components/mana_item.hpp"
#include "ecs/components/attack/attack.hpp"
#include "ecs/components/attack/swing.hpp"

namespace prefabs {
	static constexpr float TILESIZE = 16.0f / 256.0f;
	
	entt::entity player(entt::registry& registry) {
		entt::entity e = registry.create();
		registry.emplace<Position>(e, glm::vec2(0.0f, 0.0f));
		registry.emplace<Sprite>(e, glm::vec2(24.0f), glm::vec4(TILESIZE * 8.0f, TILESIZE * 1.0f, TILESIZE, TILESIZE), 0.0f, glm::vec2(0.0f));
		registry.emplace<Player>(e, 28.0f, 0.0f, 3.0f);
		return e;
	}

	entt::entity base_enemy(entt::registry& registry, glm::vec2 pos) {
		// config
		const float hp = 3.0f;

		entt::entity e = registry.create();
		registry.emplace<Position>(e, pos);
		registry.emplace<Enemy>(e, hp, hp);
		registry.emplace<Sprite>(e, glm::vec2(24.0f), glm::vec4(TILESIZE * 0.0f, TILESIZE * 5.0f, TILESIZE, TILESIZE), 0.0f, glm::vec2(0.0f));
		registry.emplace<DamageZone>(e, 2.0f, 15.0f, DamageLayer::PLAYER);
		return e;
	}

	entt::entity item_mana(entt::registry& registry) {
		entt::entity e = registry.create();
		registry.emplace<Position>(e, glm::vec2(100.0f, -300.0f));
		registry.emplace<ManaItem>(e, 1.0f, -1, glm::vec2());
		registry.emplace<Sprite>(e, glm::vec2(16.0f), glm::vec4(TILESIZE * 10.0f, TILESIZE * 2.0f, TILESIZE, TILESIZE), 0.0f, glm::vec2(0.0f));
		return e;
	}

	entt::entity attack_swing(entt::registry& registry, glm::vec2 pos, glm::vec2 dir) {
		entt::entity e = registry.create();
		registry.emplace<Position>(e, pos + dir * 27.0f * glm::vec2(0.8f, 0.9f));
		registry.emplace<Sprite>(e, glm::vec2(16.0f, 32.0f), glm::vec4(TILESIZE * 9.0f, TILESIZE * 12.0f, TILESIZE, TILESIZE * 2.0f), 0.0f, glm::vec2(0.0f, 1.0f));
		registry.emplace<Attack>(e, 0.0f, 0.75f);
		registry.emplace<AttackSwing>(e, glm::two_pi<float>() * 0.55f, glm::orientedAngle(dir * glm::vec2(1.0f, -1.0f), glm::vec2(0.0f, -1.0f)));
		registry.emplace<DamageZone>(e, 10.0f, 20.0f, DamageLayer::ENEMY);
		return e;
	}
}

