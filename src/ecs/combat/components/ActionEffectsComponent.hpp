#pragma once
#include <variant>
#include <vector>

struct HealingEffect {
        int power;
};

enum class ElementType { Neutral, Fire, Water, Electro, Earth, Wind };

struct DamageEffect {
        int power;
        ElementType element;
        bool magical;
};

struct CostMPEffect {
        int mpCost;
};

using ActionEffect = std::variant<DamageEffect, HealingEffect, CostMPEffect>;

struct ActionEffectsComponent {
        std::vector<ActionEffect> actionEffects;
};
