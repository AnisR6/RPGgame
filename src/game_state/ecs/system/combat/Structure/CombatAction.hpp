#pragma once
#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/world/World.hpp"

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
	Engine::ECS::Entity actorEntity;
	std::vector<Engine::ECS::Entity> targetEntities;
	ActionType actionType = ActionType::BASE_ATTACK;
	// actionId for to identify which spell / skill
	int actionId = -1;

	CombatAction(const Engine::ECS::Entity actor, const ActionType type, std::vector<Engine::ECS::Entity> targets)
	    : actorEntity(actor), targetEntities{std::move(targets)}, actionType(type)
	{
	}

	void evalCombatAction();
	bool operator<(const CombatAction &other) const noexcept;
	bool operator>(const CombatAction &other) const noexcept;
};
