#pragma once
#include "game_state/ecs/capabilities/Combatable.hpp"
#include <utility>
#include <vector>

enum ActionType { BASE_ATTACK, SPELL, SKILL, INVENTORY, FLIGHT };

struct CombatAction {
  private:
	void handleBaseAttack();
	void handleSpell();
	void handleSkill();
	void handleInventory();
	void handleFlight();

  public:
	Combatable *actor = nullptr;
	std::vector<Combatable *> targets;
	ActionType actionType = BASE_ATTACK;

	CombatAction(Combatable *actor, const ActionType type, std::vector<Combatable *> targets)
	    : actor(actor), targets{std::move(targets)}, actionType(type)
	{
	}
	CombatAction() = default;

	void evalCombatAction();
	bool operator<(const CombatAction &other) const noexcept;
};
