#include "BattleMapper.hpp"
#include <iterator>

CombatAction BattleMapper::mapToCombatAction(const CombatActionRequest &request)
{
	// map ID to actor
	// Combatable* actor = request.actorId;

	std::vector<Combatable *> targets;
	for (const auto &target : request.targetIds) {
		// fetch target by id
	}

	return CombatAction{nullptr, request.actionType, std::move(targets)};
}

CombatantInfo BattleMapper::mapToCombatantInfo(const PartyMember &partyMember)
{
	const Stats &stats = partyMember.getStats();
	return CombatantInfo{.name = partyMember.getName(),
	                     .currHP = stats.currHP,
	                     .maxHP = stats.maxHP,
	                     .currMP = stats.currMP,
	                     .maxMP = stats.maxMP,
	                     .spells = partyMember.getSpells(),
	                     .skills = partyMember.getCombatSkills()};
}

CombatantInfo BattleMapper::mapToCombatantInfo(const Enemy &enemy)
{
	const Stats &stats = enemy.getStats();
	return CombatantInfo{.name = enemy.getName(),
	                     .currHP = stats.currHP,
	                     .maxHP = stats.maxHP,
	                     .currMP = stats.currMP,
	                     .maxMP = stats.maxMP,
	                     .spells = enemy.getSpells(),
	                     .skills = enemy.getCombatSkills()};
}

template <typename T>
std::vector<CombatantInfo> BattleMapper::mapToCombatantInfos(const std::vector<T> &combatants)
{
	std::vector<CombatantInfo> result;
	result.reserve(combatants.size());
	std::ranges::transform(combatants, std::back_inserter(result), [](const T &c) { return mapToCombatantInfo(c); });
	return result;
}