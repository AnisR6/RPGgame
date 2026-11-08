#include "BattleSystem.hpp"
#include <algorithm>

void BattleSystem::handlePlayerInputForCurrentPartyMember(ActionType type, std::vector<Combatable *> targets,
                                                          const Spell *spell, const CombatSkill *skill)
{
	auto &party = battleState.getPartyMembers();
	auto &currentPartyMember = party[currentPartyMemberIndex];

	if (type == ActionType::SPELL && spell != nullptr) {
		roundActions.emplace_back(CombatAction::createSpellAction(&currentPartyMember, std::move(targets), *spell));
	} else if (type == ActionType::SKILL && skill != nullptr) {
		roundActions.emplace_back(CombatAction::createSkillAction(&currentPartyMember, std::move(targets), *skill));
	} else {
		roundActions.emplace_back(&currentPartyMember, type, std::move(targets));
	}

	currentPartyMemberIndex++;

	if (currentPartyMemberIndex >= party.size()) {
		currentPartyMemberIndex = 0;
		executeRound();
	}
}

void BattleSystem::getEnemyActions()
{
	for (auto &enemy : battleState.getEnemies()) {
		// consult enemy ai to choose an action
	}
}

void BattleSystem::executeRound()
{
	getEnemyActions();

	// sort actions by speed
	std::ranges::sort(roundActions, std::greater<CombatAction>{});

	for (auto &action : roundActions) {
		if (action.actor != nullptr && action.actor->isAlive()) {
			action.evalCombatAction();
		}

		if (battleState.won() || battleState.lost()) {
			break;
		}
	}
	roundActions.clear();
}

const std::vector<PartyMember> &BattleSystem::getPartyMembers() const
{
	return battleState.getPartyMembers();
}

const std::vector<Enemy> &BattleSystem::getEnemies() const
{
	return battleState.getEnemies();
}