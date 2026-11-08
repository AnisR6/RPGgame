#pragma once
#include <string>

enum class SpellType { FIRE, LIGHTNING, WIND, HEALTH, STATUS };
enum class TargetScope { SINGLE_ENEMY, ALL_ENEMIES, SINGLE_ALLIE, ALL_ALLIES, SELF };

struct Spell {
	int spellId;
	std::string spellName;
	SpellType spellType;
	TargetScope targetScope;
	int consumedMP;
	int power;
};