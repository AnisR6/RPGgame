#include "BattleMapper.hpp"

#include "game_state/ecs/components/CombatSkillComponent.hpp"
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/SpellComponent.hpp"
#include "game_state/ecs/components/combat/StatsComponent.hpp"

#include <iterator>

#include <algorithm>
#include <iterator>

CombatAction BattleMapper::mapToCombatAction(const CombatActionRequest &request)
{
	return CombatAction{request.actorEntity, request.actionType, request.targetEntities};
}

CombatantInfo BattleMapper::mapToCombatantInfo(const Engine::ECS::Entity entity) const
{
	const auto &[name] = world.GetComponent<NameComponent>(entity);
	const StatsComponent &stats = world.GetComponent<StatsComponent>(entity);
	const SpellComponent &spells = world.GetComponent<SpellComponent>(entity);
	const CombatSkillComponent &skills = world.GetComponent<CombatSkillComponent>(entity);

	// TODO: Map to objects
	/*
	for (auto spellId : spells.spellIds) {

	}

	for (auto skillId : skills.skillIds) {

	}
	*/

	return CombatantInfo{.name = name,
	                     .currHP = stats.currHP,
	                     .maxHP = stats.maxHP,
	                     .currMP = stats.currMP,
	                     .maxMP = stats.maxMP,
	                     .spells = {},
	                     .skills = {}};
}

std::vector<CombatantInfo>
BattleMapper::mapToCombatantInfos(const std::vector<Engine::ECS::Entity> &combatantEntities) const
{
	std::vector<CombatantInfo> result;
	result.reserve(combatantEntities.size());
	std::ranges::transform(combatantEntities, std::back_inserter(result),
	                       [this](const Engine::ECS::Entity e) { return mapToCombatantInfo(e); });
	return result;
}