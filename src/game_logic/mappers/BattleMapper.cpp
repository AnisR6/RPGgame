#include "BattleMapper.hpp"

#include "game_state/ecs/system/combat/combat_action/CombatAction.hpp"

CombatAction BattleMapper::mapToCombatAction(const CombatActionRequest &request)
{
	return CombatAction{.actorEntity = request.actorEntity,
	                    .targetEntities = request.targetEntities,
	                    .actionEntity = request.actionEntity};
}