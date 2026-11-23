#include "BattleMapper.hpp"

#include "ecs/system/combat/combat_action/CombatAction.hpp"

CombatAction BattleMapper::mapToCombatAction(const CombatActionRequest &request)
{
	return CombatAction{.actorEntity = request.actorEntity,
	                    .targetEntities = request.targetEntities,
	                    .actionEntity = request.actionEntity};
}