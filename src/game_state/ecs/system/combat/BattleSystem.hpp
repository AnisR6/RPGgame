#pragma once
#include "Structure/CombatAction.hpp"
#include "game_state/ecs/system/ISystem.hpp"

namespace Engine::ECS {

// TODO: Split into multiple smaller systems
/*
class BattleSystem : public ISystem {

  public:
    void handlePlayerInputForCurrentPartyMember(const CombatAction &action);
    void Update(SystemContext &context, float deltaTime) override;

  private:
    int currentPartyMemberIndex = 0;
    std::vector<CombatAction> roundActions;

    void getEnemyActions();
    void executeRound();
};
*/
} // namespace Engine::ECS