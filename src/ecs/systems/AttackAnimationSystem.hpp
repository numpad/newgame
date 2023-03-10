#pragma once

#include <bx/easing.h>
#include <entt/entt.hpp>
#include "ecs/components/attack/attack.hpp"
#include "ecs/components/attack/swing.hpp"
#include "ecs/components/sprite.hpp"
#include "ecs/components/deletable.hpp"

class AttackAnimationSystem {
private:
	entt::registry& m_registry;

public:

	AttackAnimationSystem(entt::registry& registry) : m_registry{registry} {

	}

	void tick() const {
		auto attacks = m_registry.view<Attack>();

		attacks.each([&](const entt::entity attackEntity, Attack& attack) {
			// general info
			Sprite* sprite = m_registry.try_get<Sprite>(attackEntity);

			// handle specific logic

			AttackSwing* swing = m_registry.try_get<AttackSwing>(attackEntity);
			if (swing) {
				sprite->angle = swing->angle + swing->max_angle * (bx::easeOutExpo(attack.lifetime / (attack.max_lifetime)) - 0.5f);
			}

			// handle attack
			attack.lifetime += (1.0f / 60.0f);
			if (attack.lifetime >= attack.max_lifetime) {
				attack.lifetime = attack.max_lifetime;
				m_registry.emplace_or_replace<Deletable>(attackEntity);
			}
		});
	}

	void update() const {

	}
};

