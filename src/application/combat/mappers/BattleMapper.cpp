#include "application/combat/mappers/BattleMapper.hpp"

CombatAction BattleMapper::MapToCombatAction(const CombatActionRequest& request) {
    return CombatAction{.actorEntity = request.actorEntity,
                        .targetEntities = request.targetEntities,
                        .actionEntity = request.actionEntity};
}
