#include "BattleController.hpp"

#include <utility>

#include "application/messages/BattleMessage.hpp"
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/combat/BaseAttackComponent.hpp"
#include "game_state/ecs/components/combat/CombatSkillComponent.hpp"
#include "game_state/ecs/components/combat/FlightComponent.hpp"
#include "game_state/ecs/components/combat/KnownActionsComponent.hpp"
#include "game_state/ecs/components/combat/SpellComponent.hpp"
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
	currentActorIndex = response.currentActorIndex;

	setBattleInfoForBattleScreen();
	setCallbacksForBattleScreen();
	setBattleActorMenuDataForBattleScreen();
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

CombatantMenuEntry BattleController::createTargetEntry(const Engine::ECS::Entity targetEntity) const
{
	auto [name] = world.GetComponent<NameComponent>(targetEntity);
	return {.entity = targetEntity, .name = name};
}

void BattleController::showBattleDialog(const ShowBattleDialogRequest &request) const
{
	const auto &actorName = world.GetComponent<NameComponent>(request.actorEntity).name;
	const auto &actionName = world.GetComponent<NameComponent>(request.actionEntity).name;

	// TODO: add a DialogManager later for more complex messages
	BattleScreen::getInstance().showBattleDialog(actorName + "uses" + actionName + ".",
	                                             [this]() { battleMessageBus.publish(ShowBattleDialogFinished{}); });
}

void BattleController::playBattleAnimation(const PlayBattleAnimationRequest &request) const
{
	// placeholder for now until animations are implemented
	battleMessageBus.publish(PlayBattleAnimationFinished{});
}

void BattleController::setBattleInfoForBattleScreen() const
{
	std::vector<CombatantMenuEntry> playerPartyEntries;
	std::vector<CombatantMenuEntry> enemyEntries;

	for (const auto &partyMember : partyMembers) {
		playerPartyEntries.push_back(createTargetEntry(partyMember));
	}

	for (const auto &enemy : enemies) {
		enemyEntries.push_back(createTargetEntry(enemy));
	}

	BattleScreen::getInstance().setBattleInfo(playerPartyEntries, enemyEntries, currentActorIndex);
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

void BattleController::setBattleActorMenuDataForBattleScreen() const
{
	const auto actor = partyMembers.at(currentActorIndex);
	const auto knownActionsEntities = world.GetComponent<KnownActionsComponent>(actor).knownActionEntities;

	std::vector<BattleActionMenuEntry> battleActionMenuEntries;
	BattleActionMenuEntry baseAttack;
	BattleActionMenuEntry flight;
	std::vector<BattleActionMenuEntry> spells;
	std::vector<BattleActionMenuEntry> skills;

	for (const auto knownAction : knownActionsEntities) {
		if (world.HasComponent<BaseAttackComponent>(knownAction)) {
			baseAttack = createActionEntry(knownAction);
		} else if (world.HasComponent<FlightComponent>(knownAction)) {
			flight = createActionEntry(knownAction);
		} else if (world.HasComponent<SpellComponent>(knownAction)) {
			spells.push_back(createActionEntry(knownAction));
		} else if (world.HasComponent<CombatSkillComponent>(knownAction)) {
			skills.push_back(createActionEntry(knownAction));
		}
	}

	BattleActorMenuData data{
	    baseAttack,
	    flight,
	    spells,
	    skills,
	};

	BattleScreen::getInstance().setBattleActorMenuData(std::move(data));
}
