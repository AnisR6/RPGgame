#pragma once
#include "TargetScope.hpp"
#include <string>

enum class SpellType { FIRE, LIGHTNING, WIND, HEALTH, STATUS };

struct Spell {
	int spellId;
	std::string spellName;
	SpellType spellType;
	TargetScope targetScope;
	int consumedMP;
	int power;
};