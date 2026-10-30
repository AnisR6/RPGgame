#pragma once
#include <string>

enum SpellType { FIRE, LIGHTNING, WIND, HEALTH, STATUS };
enum TargetScope { SINGLE_ENEMY, ALL_ENEMIES, SINGLE_ALLIE, ALL_ALLIES, SELF };

struct Spell {
	unsigned spellId;
	std::string spellName;
	SpellType spellType;
	TargetScope targetScope;
	unsigned consumedMP;
	unsigned power;
};