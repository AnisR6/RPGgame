#include "BattleMapper.hpp"

CombatAction BattleMapper::mapToCombatAction(const CombatActionRequest &request)
{
	return CombatAction{request.actor, request.actionType, std::move(request.targets)};
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