#pragma once

enum SpellType { FIRE, LIGHTNING, WIND, HEALTH, STATUS };

struct Spell {
	SpellType spellType;
	unsigned consumedMP;
};