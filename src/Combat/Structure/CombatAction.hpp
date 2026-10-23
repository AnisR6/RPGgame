#pragma once
#include <memory>

#include "Ability/Combatable.hpp"


class Combatable;

enum ActionType {
    BASE_ATTACK,
    SPELL,
    SKILL,
    INVENTORY,
    FLIGHT
};


struct CombatAction {
	Combatable* actor;
    std::vector<std::unique_ptr<Combatable>> targets;
    ActionType actionType;


    bool operator<(const CombatAction& other) const noexcept {
    }
};
