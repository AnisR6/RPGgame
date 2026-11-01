#pragma once
#include "game_state/ecs/capabilities/Combatable.hpp"
#include <utility>
#include <vector>

enum class ActionType { BASE_ATTACK, SPELL, SKILL, INVENTORY, FLIGHT };

struct CombatAction {
  private:
	void handleBaseAttack() const;
	void handleSpell();
	void handleSkill() const;
	void handleInventory();
	void handleFlight();

	template <typename F>
	void applyToTargets(F &&func) const;

	static int computePhysicalDamage(int actor_atk, int target_def);
	static int computeMagicalDamage(int actor_mag, int target_res);

	void handleAttackSpell() const;
	void handleHealthSpell() const;
	void handleStatusSpell();

  public:
	Combatable *actor = nullptr;
	std::vector<Combatable *> targets;
	ActionType actionType = ActionType::BASE_ATTACK;
	const Spell *spell = nullptr;
	const CombatSkill *skill = nullptr;

	CombatAction(Combatable *actor, const ActionType type, std::vector<Combatable *> targets)
	    : actor(actor), targets{std::move(targets)}, actionType(type)
	{
	}

	static CombatAction createSpellAction(Combatable *actor, std::vector<Combatable *> targets, const Spell &spell)
	{
		CombatAction action{actor, ActionType::SPELL, std::move(targets)};
		action.spell = &spell;
		return action;
	}
	static CombatAction createSkillAction(Combatable *actor, std::vector<Combatable *> targets,
	                                      const CombatSkill &skill)
	{

		CombatAction action{actor, ActionType::SKILL, std::move(targets)};
		action.skill = &skill;
		return action;
	}

	void evalCombatAction();
	bool operator<(const CombatAction &other) const noexcept;
	bool operator>(const CombatAction &other) const noexcept;
};
