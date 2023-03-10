#pragma once

#include <unordered_set>
#include <entt/entt.hpp>

enum DamageLayer {
	PLAYER = 1 << 0,
	ENEMY  = 1 << 1
};

inline DamageLayer operator|(DamageLayer a, DamageLayer b) {
	return static_cast<DamageLayer>(static_cast<int>(a) | static_cast<int>(b));
}

inline DamageLayer operator&(DamageLayer a, DamageLayer b) {
	return static_cast<DamageLayer>(static_cast<int>(a) & static_cast<int>(b));
}

struct DamageZone {
	float dps;
	float radius;
	DamageLayer layer;
	std::unordered_set<entt::entity> entities_hit;
	int tickinterval;
};

