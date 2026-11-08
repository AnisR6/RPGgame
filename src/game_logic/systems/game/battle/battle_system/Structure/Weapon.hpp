#pragma once
#include "TargetScope.hpp"

enum WeaponType { SWORD, LANCE, AXE, STAFF };

struct Weapon {
	WeaponType type;
	unsigned baseDamage;
	TargetScope targetScope;
	float critRate;
};