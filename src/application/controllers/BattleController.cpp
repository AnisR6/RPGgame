#include "BattleController.hpp"

#include <utility>

#include "application/messages/BattleMessage.hpp"
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/combat/StatsComponent.hpp"
#include "presentation/screens/battle_screen/BattleScreen.hpp"

BattleController::BattleController(BattleMessageBus &messageBus, Engine::ECS::World &world)
    : battleMessageBus(messageBus), world(world)
{
	messageBus.subscribe<BattleInfoResponse>(
	    [this](const BattleInfoResponse &response) { updateBattleInfo(response); });

	messageBus.subscribe<ShowBattleDialogRequest>(
	    [this](const ShowBattleDialogRequest &request) { showBattleDialog(request); });

	messageBus.subscribe<PlayBattleAnimationRequest>(
	    [this](const PlayBattleAnimationRequest &request) { playBattleAnimation(request); });
	;
}

// TODO: adjust to entity based approach
void BattleController::updateBattleInfo(const BattleInfoResponse &response)
{
	// Extract Data of the entities that will be displayed in the screen
	partyMembers = response.playerPartyEntities;
	enemies = response.enemyPartyEntities;

	setBattleInfoForBattleScreen();
	setCallbacksForBattleScreen();
}

// fetching initial battle data
void BattleController::initBattle() const
{
	battleMessageBus.publish(BattleInfoRequest{});
}

// create entries for UI
BattleActionMenuEntry BattleController::createActionEntry(const Engine::ECS::Entity actionEntity) const
{
	auto [name] = world.GetComponent<NameComponent>(actionEntity);
	auto [targetScope] = world.GetComponent<TargetScopeComponent>(actionEntity);
	return {.entity = actionEntity, .name = name, .targetScope = targetScope};
}

CombatantMenuEntry BattleController::createTargetEntry(const Engine::ECS::Entity targetEntity)
{
	auto [name] = world.GetComponent<NameComponent>(targetEntity);
	return {.entity = targetEntity, .name = name};
}

void BattleController::showBattleDialog(const ShowBattleDialogRequest &request) {}

void BattleController::playBattleAnimation(const PlayBattleAnimationRequest &request)
{
	// Animations implemented later
}

void BattleController::setBattleInfoForBattleScreen() const
{
	// TODO: change this, not working with ecs
	// BattleScreen::getInstance().setBattleInfo(partyMembers, enemies, currentPartyMemberIndex);
}

void BattleController::setCallbacksForBattleScreen()
{
	callbacks.onActionSelected = [this](std::vector<Engine::ECS::Entity> targetEntities,
	                                    const Engine::ECS::Entity actionEntity) {
		const auto actor = partyMembers.at(currentActorIndex);

		battleMessageBus.publish(CombatActionRequest{
		    .actorEntity = actor, .targetEntities = std::move(targetEntities), .actionEntity = actionEntity});
	};

	BattleScreen::getInstance().setCallbacks(std::move(callbacks));
}
