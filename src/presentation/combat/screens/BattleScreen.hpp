#pragma once
#include "application/combat/callbacks/BattleMenuCallbacks.hpp"
#include "application/combat/menu_entries/BattleActionMenuEntry.hpp"
#include "application/combat/menu_entries/BattleActorMenuData.hpp"
#include "application/combat/menu_entries/CombatantMenuEntry.hpp"
#include "ecs/combat/components/TargetScopeComponent.hpp"
#include "presentation/shared/screens/Screen.hpp"

class BattleScreen : public Screen {
    private:
        std::vector<CombatantMenuEntry> playerParty;
        std::vector<CombatantMenuEntry> enemies;
        std::size_t currentPartyMemberIndex = 0;

        BattleActorMenuData battleActorMenuData;

        BattleMenuCallbacks callbacks;

        std::unique_ptr<Widget> CreateMainBattleMenu();

        std::unique_ptr<Widget> CreateSubMenu(const std::vector<BattleActionMenuEntry>& entries);

        std::unique_ptr<Widget> CreateTargetMenu(
            TargetScope targetScope,
            const std::function<void(std::vector<Engine::ECS::Entity>)>& onTargetSelected);

        void SelectAction(const BattleActionMenuEntry& action);
        void ShowMain();
        void ShowSpellMenu();
        void ShowSkillMenu();
        void ShowTargetMenu(
            TargetScope targetScope,
            const std::function<void(std::vector<Engine::ECS::Entity>)>& onTargetSelected);

        [[nodiscard]] std::optional<std::string>
        GetTargetLabel(TargetScope scope, const std::vector<Engine::ECS::Entity>& targets) const;

    public:
        BattleScreen();
        void SetCallbacks(BattleMenuCallbacks newCallbacks);
        void SetBattleInfo(std::vector<CombatantMenuEntry> newPlayerParty,
                           std::vector<CombatantMenuEntry> newEnemies,
                           std::size_t partyMemberIndex);
        void SetBattleActorMenuData(BattleActorMenuData data);

        void ShowBattleDialog(const std::string& battleDialogMessage,
                              std::function<void()> onFinished);
        void PlayBattleAnimation();

        BattleScreen(const BattleScreen&) = delete;
        BattleScreen& operator=(const BattleScreen&) = delete;
};
