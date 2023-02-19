#pragma once

#include <entt/entt.hpp>
#include "ecs/components/deletable.hpp"

class DeleteSystem {
private:
	entt::registry& m_registry;

public:

	DeleteSystem(entt::registry& registry) : m_registry{registry} {

	}

	void tick() const {
		m_registry.view<Deletable>().each([&](const entt::entity e) {
			m_registry.destroy(e);
		});
	}
};

