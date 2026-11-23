#pragma once
#include <variant>
#include <vector>

struct HealingEffect {
	int power;
};

enum class ElementType { NEUTRAL, FIRE, WATER, ELECTRO, EARTH, WIND };

struct DamageEffect {
	int power;
	ElementType element;
	bool magical;
};

struct CostMPEffect {
	int mpCost;
};

// more effects to come

using ActionEffect = std::variant<DamageEffect, HealingEffect, CostMPEffect>;

struct ActionEffectsComponent {
	std::vector<ActionEffect> actionEffects;
};
