#pragma once
#include <variant>
#include <vector>

struct HealingEffect {
	int power;
};

enum class ElementType { NEUTRAL, FIRE, WATER, ELECTRO, EARTH, WIND };

struct DamageEffect {
	int power;
	ElementType element = ElementType::NEUTRAL;
	bool magical;
};

// more effects to come

using ActionEffect = std::variant<DamageEffect, HealingEffect>;

struct ActionEffectsComponent {
	std::vector<ActionEffect> actionEffects;
};
