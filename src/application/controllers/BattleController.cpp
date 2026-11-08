#include "BattleController.hpp"

#include <utility>

#include "application/messages/BattleMessage.hpp"
#include "presentation/screens/battle_screen/BattleScreen.hpp"

BattleController::BattleController(BattleMessageBus &messageBus) : battleMessageBus(messageBus)
{
	messageBus.subscribe<BattleInfoResponse>(
	    [this](const BattleInfoResponse &response) { updateBattleInfo(response); });
}

void BattleController::updateBattleInfo(const BattleInfoResponse &response)
{
	partyMembers = response.playerPartyInfo;
	enemies = response.enemyPartyInfo;
	currentPartyMemberIndex = response.currentPartyMemberIndex;

	setBattleInfoForBattleScreen();
}

void BattleController::initBattle()
{
	setCallbacksForBattleScreen();
	battleMessageBus.publish(BattleInfoRequest{});
}

void BattleController::setBattleInfoForBattleScreen() const
{
	BattleScreen::getInstance().setBattleInfo(partyMembers, enemies, currentPartyMemberIndex);
}

void BattleController::setCallbacksForBattleScreen()
{
	callbacks.onBaseAttack = [this](std::vector<int> targetIds) {
		CombatActionRequest baseAttackRequest{
		    .targets = std::move(targetIds), .actionType = ActionType::BASE_ATTACK, .actionId = -1};

		battleMessageBus.publish(baseAttackRequest);
	};

	callbacks.onSpell = [this](std::vector<int> targetIds, const int spellId) {
		CombatActionRequest spellRequest{
		    .targets = std::move(targetIds), .actionType = ActionType::SPELL, .actionId = spellId

		};

		battleMessageBus.publish(spellRequest);
	};

	callbacks.onSkill = [this](std::vector<int> targetIds, const int skilLId) {
		CombatActionRequest skillRequest{
		    .targets = std::move(targetIds), .actionType = ActionType::SKILL, .actionId = skilLId};

		battleMessageBus.publish(skillRequest);
	};

	callbacks.onInventory = [this](std::vector<int> targetIs, const int itemId) {
		CombatActionRequest inventoryRequest{
		    .targets = std::move(targetIs), .actionType = ActionType::INVENTORY, .actionId = itemId};

		battleMessageBus.publish(inventoryRequest);
	};

	callbacks.onFlight = [this]() {
		CombatActionRequest flightRequest{};

		battleMessageBus.publish(flightRequest);
	};

	BattleScreen::getInstance().setCallbacks(std::move(callbacks));
}
