#include "BattleController.hpp"

#include "application/messages/BattleMessage.hpp"
#include "presentation/screens/battle_screen/BattleScreen.hpp"

BattleController::BattleController(BattleMessageBus &eventBus) : battleMessageBus(eventBus)
{
	eventBus.subscribe<BattleInfoResponse>([this](const BattleInfoResponse &response) { updateBattleInfo(response); });
}

void BattleController::updateBattleInfo(const BattleInfoResponse &response)
{
	partyMembers = response.playerPartyInfo;
	enemies = response.enemyPartyInfo;
	setBattleInfoForBattleScreen();
}

void BattleController::initBattle()
{
	setCallbacksForBattleScreen();
	battleMessageBus.publish(BattleInfoRequest{});
}

void BattleController::setBattleInfoForBattleScreen()
{
	// TODO: handle currentPartyMemberIndex (last argument)
	// BattleScreen::getInstance().setBattleInfo(partyMembers, enemies, );
}

void BattleController::setCallbacksForBattleScreen()
{

	callbacks.onBaseAttack = [this]() {
		/*
		CombatActionRequest baseAttackRequest{partyMembers.at(currentPartyMemberIndex), targets,
		                                      ActionType::BASE_ATTACK, -1, -1};

		battleMessageBus.publish(baseAttackRequest);
		*/
	};

	callbacks.onSpell = [this](int spellId) {
		CombatActionRequest spellRequest{

		};

		battleMessageBus.publish(spellRequest);
	};

	callbacks.onSkill = [this](int skilLId) {
		CombatActionRequest skillRequest{

		};

		battleMessageBus.publish(skillRequest);
	};

	callbacks.onInventory = [this](int itemId) {
		CombatActionRequest inventoryRequest{

		};

		battleMessageBus.publish(inventoryRequest);
	};

	callbacks.onFlight = [this]() {
		CombatActionRequest flightRequest{

		};

		battleMessageBus.publish(flightRequest);
	};

	BattleScreen::getInstance().setCallbacks(std::move(callbacks));
}
