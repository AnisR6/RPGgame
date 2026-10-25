#pragma once
#include "Ability/Combatable.hpp"
#include <utility>
#include <vector>

enum ActionType { BASE_ATTACK, SPELL, SKILL, INVENTORY, FLIGHT };

struct CombatAction {
	Combatable *actor = nullptr;
	std::vector<Combatable *> targets;
	ActionType actionType = BASE_ATTACK;

	CombatAction(Combatable *actor, const ActionType type, std::vector<Combatable *> targets)
	    : actor(actor), targets{std::move(targets)}, actionType(type)
	{
	}
	CombatAction() = default;

	bool operator<(const CombatAction &other) const noexcept;
};
