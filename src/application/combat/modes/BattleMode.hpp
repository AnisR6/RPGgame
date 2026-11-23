#pragma once

#include "application/combat/bus/BattleMessageBus.hpp"
#include "application/combat/controllers/BattleController.hpp"
#include "application/shared/modes/IMode.hpp"
#include "ecs/core/systems/SystemManager.hpp"
#include "ecs/core/world/World.hpp"
#include "ecs/combat/systems/BattleState.hpp"
#include "ecs/combat/systems/CombatActionExecutor.hpp"
#include "presentation/combat/screens/BattleScreen.hpp"

#include <vector>

class Renderer;

class BattleMode : public IMode {
    public:
        BattleMode(Engine::ECS::World& world, Renderer& renderer);

        void OnEnter() override;
        void OnExit() override;
        bool HandleEvent(const sf::Event& event) override;
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;

    private:
        void SubscribeToEvents();
        void RegisterSystems();

        void StartBattle();
        void EndBattle() const;

        void HandleBattleInfoRequest() const;
        void HandleCombatActionRequest(const CombatActionRequest& combatActionRequest);

        void CreateParties(std::vector<Engine::ECS::Entity>& partyMembers,
                           std::vector<Engine::ECS::Entity>& enemies) const;

        BattleMessageBus messageBus;
        BattleScreen screen;
        Engine::ECS::World& world;
        Renderer& renderer;
        Engine::ECS::SystemManager systemManager;
        BattleState battleState;
        CombatActionExecutor executor;
        BattleController controller;
};
