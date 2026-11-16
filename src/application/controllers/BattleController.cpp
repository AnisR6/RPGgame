#include "BattleController.hpp"

#include <utility>

#include "application/messages/BattleMessage.hpp"
#include "game_state/ecs/components/combat/StatsComponent.hpp"
#include "presentation/screens/battle_screen/BattleScreen.hpp"

BattleController::BattleController(BattleMessageBus &messageBus, Engine::ECS::World &world)
    : battleMessageBus(messageBus), world(world)
{
	messageBus.subscribe<BattleInfoResponse>(
	    [this](const BattleInfoResponse &response) { updateBattleInfo(response); });
}

// TODO: adjust to entity based approach
void BattleController::updateBattleInfo(const BattleInfoResponse &response)
{
	// Extract Data of the entities that will be displayed in the screen
	partyMembers = response.playerPartyEntities;
	enemies = response.enemyPartyEntities;

	setBattleInfoForBattleScreen();
}

void BattleController::initBattle()
{
	setCallbacksForBattleScreen();
	battleMessageBus.publish(BattleInfoRequest{});
}

void BattleController::setBattleInfoForBattleScreen() const
{
	// TODO: change this, not working with ecs
	// BattleScreen::getInstance().setBattleInfo(partyMembers, enemies, currentPartyMemberIndex);
}

void BattleController::setCallbacksForBattleScreen()
{
	auto actor = partyMembers.at(currentActorIndex);

	callbacks.onBaseAttack = [this, actor](std::vector<Engine::ECS::Entity> targetIds) {
		CombatActionRequest baseAttackRequest{.actorEntity = actor,
		                                      .targetEntities = std::move(targetIds),
		                                      .actionType = ActionType::BASE_ATTACK,
		                                      .actionId = -1};

		battleMessageBus.publish(baseAttackRequest);
	};

	callbacks.onSpell = [this, actor](std::vector<Engine::ECS::Entity> targetIds, const int spellId) {
		CombatActionRequest spellRequest{.actorEntity = actor,
		                                 .targetEntities = std::move(targetIds),
		                                 .actionType = ActionType::SPELL,
		                                 .actionId = spellId

		};

		battleMessageBus.publish(spellRequest);
	};

	callbacks.onSkill = [this, actor](std::vector<Engine::ECS::Entity> targetIds, const int skilLId) {
		CombatActionRequest skillRequest{.actorEntity = actor,
		                                 .targetEntities = std::move(targetIds),
		                                 .actionType = ActionType::SKILL,
		                                 .actionId = skilLId};

		battleMessageBus.publish(skillRequest);
	};

	callbacks.onInventory = [this, actor](std::vector<Engine::ECS::Entity> targetIs, const int itemId) {
		CombatActionRequest inventoryRequest{.actorEntity = actor,
		                                     .targetEntities = std::move(targetIs),
		                                     .actionType = ActionType::INVENTORY,
		                                     .actionId = itemId};

		battleMessageBus.publish(inventoryRequest);
	};

	callbacks.onFlight = [this]() {
		CombatActionRequest flightRequest{};

		battleMessageBus.publish(flightRequest);
	};

	BattleScreen::getInstance().setCallbacks(std::move(callbacks));
}
