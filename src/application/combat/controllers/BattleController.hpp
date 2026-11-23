#pragma once

#include "application/combat/bus/BattleMessageBus.hpp"
#include "application/combat/callbacks/BattleMenuCallbacks.hpp"
#include "application/combat/menu_entries/BattleActionMenuEntry.hpp"
#include "application/combat/menu_entries/CombatantMenuEntry.hpp"
#include "ecs/core/world/World.hpp"
#include "presentation/combat/screens/BattleScreen.hpp"

class BattleController {
    private:
        BattleMessageBus& messageBus;
        BattleScreen& screen;
        Engine::ECS::World& world;
        std::vector<Engine::ECS::Entity> partyMembers;
        std::vector<Engine::ECS::Entity> enemies;
        std::size_t currentActorIndex{};
        BattleMenuCallbacks callbacks;

        void SetCallbacksForBattleScreen();
        void SetBattleInfoForBattleScreen() const;
        void SetBattleActorMenuDataForBattleScreen() const;
        void UpdateBattleInfo(const BattleInfoResponse& response);

        [[nodiscard]] BattleActionMenuEntry
        CreateActionEntry(Engine::ECS::Entity actionEntity) const;
        [[nodiscard]] CombatantMenuEntry
        CreateCombatantMenuEntry(Engine::ECS::Entity combatantEntity) const;

        void ShowBattleDialog(const ShowBattleDialogRequest& request) const;
        void PlayBattleAnimation(const PlayBattleAnimationRequest& request) const;

    public:
        BattleController(BattleMessageBus& messageBus, BattleScreen& screen,
                         Engine::ECS::World& world);
        void RequestBattleInfo() const;
};
