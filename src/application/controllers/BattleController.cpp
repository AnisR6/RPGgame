#include "BattleController.hpp"

#include "application/messages/BattleMessage.hpp"
#include "presentation/screens/battle_screen/BattleScreen.hpp"

BattleController::BattleController(BattleMessageBus &eventBus) : battleMessageBus(eventBus)
{
	// eventBus.subscribe();
}

void BattleController::initBattle()
{
	battleMessageBus.publish(BattleInfoRequest{});
	setCallbacksForBattleScreen();
}

void BattleController::setCallbacksForBattleScreen()
{

	callbacks.onBaseAttack = [this]() {

	};

	callbacks.onSpell = [this](int spellId) {

	};

	callbacks.onSkill = [this](int skilLId) {

	};

	callbacks.onInventory = [this](int itemId) {

	};

	callbacks.onFlight = [this]() {

	};

	BattleScreen::getInstance().setCallbacks(std::move(callbacks));
}
