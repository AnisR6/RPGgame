#include "BattleController.hpp"

#include <utility>

#include "application/messages/BattleMessage.hpp"
#include "ecs/components/overworld/NameComponent.hpp"
#include "ecs/components/combat/BaseAttackComponent.hpp"
#include "ecs/components/combat/CombatSkillComponent.hpp"
#include "ecs/components/combat/DeadComponent.hpp"
#include "ecs/components/combat/FleeComponent.hpp"
#include "ecs/components/combat/KnownActionsComponent.hpp"
#include "ecs/components/combat/SpellComponent.hpp"
#include "ecs/components/combat/StatsComponent.hpp"
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
	messageBus.subscribe<PlayerTurnStarted>([this](const PlayerTurnStarted &) { requestBattleInfo(); });
}

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
void BattleController::requestBattleInfo() const
{
	battleMessageBus.publish(BattleInfoRequest{});
}

// create entries for UI
BattleActionMenuEntry BattleController::createActionEntry(const Engine::ECS::Entity actionEntity) const
{
	if (!world.HasComponent<TargetScopeComponent>(actionEntity)) {
		throw std::runtime_error("BattleController: Action has no TargetScopeComponent");
	}
	auto [name] = world.GetComponent<NameComponent>(actionEntity);
	auto [targetScope] = world.GetComponent<TargetScopeComponent>(actionEntity);
	return {.entity = actionEntity, .name = name, .targetScope = targetScope};
}

CombatantMenuEntry BattleController::createCombatantMenuEntry(const Engine::ECS::Entity combatantEntity) const
{
	auto [name] = world.GetComponent<NameComponent>(combatantEntity);
	const auto &stats = world.GetComponent<StatsComponent>(combatantEntity);
	return {.entity = combatantEntity,
	        .name = name,
	        .maxHP = stats.maxHP,
	        .currHP = stats.currHP,
	        .maxMP = stats.maxMP,
	        .currMP = stats.currMP,
	        .isDead = world.HasComponent<DeadComponent>(combatantEntity)};
}

void BattleController::showBattleDialog(const ShowBattleDialogRequest &request) const
{
	const auto &actorName = world.GetComponent<NameComponent>(request.actorEntity).name;
	const auto &actionName = world.GetComponent<NameComponent>(request.actionEntity).name;

	std::string message = actorName + " uses " + actionName;

	if (!request.targetEntities.empty()) {
		message += " on ";
		for (std::size_t i = 0; i < request.targetEntities.size(); ++i) {
			if (i > 0 && i != request.targetEntities.size() - 1) {
				message += ", ";
			}
			if (i > 0 && i == request.targetEntities.size() - 1) {
				message += " and ";
			}
			message += world.GetComponent<NameComponent>(request.targetEntities.at(i)).name;
		}
	}
	message += '.';

	// TODO: add a DialogManager later for more complex messages
	BattleScreen::getInstance().showBattleDialog(message,
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
		playerPartyEntries.push_back(createCombatantMenuEntry(partyMember));
	}

	for (const auto &enemy : enemies) {
		enemyEntries.push_back(createCombatantMenuEntry(enemy));
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

	std::optional<BattleActionMenuEntry> baseAttack;
	std::optional<BattleActionMenuEntry> flee;
	std::vector<BattleActionMenuEntry> spells;
	std::vector<BattleActionMenuEntry> skills;

	for (const auto knownAction : knownActionsEntities) {
		if (world.HasComponent<BaseAttackComponent>(knownAction)) {
			baseAttack = createActionEntry(knownAction);
		} else if (world.HasComponent<FleeComponent>(knownAction)) {
			flee = createActionEntry(knownAction);
		} else if (world.HasComponent<SpellComponent>(knownAction)) {
			spells.push_back(createActionEntry(knownAction));
		} else if (world.HasComponent<CombatSkillComponent>(knownAction)) {
			skills.push_back(createActionEntry(knownAction));
		}
	}

	BattleActorMenuData data{
	    .baseAttack = baseAttack,
	    .flee = flee,
	    .spells = spells,
	    .skills = skills,
	};

	BattleScreen::getInstance().setBattleActorMenuData(std::move(data));
}
