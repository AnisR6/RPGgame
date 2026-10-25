#include "CombatAction.hpp"
#include "Ability/Combatable.hpp"

bool CombatAction::operator<(const CombatAction &other) const noexcept
{
	return actor->getStats().speed < other.actor->getStats().speed;
}

void CombatAction::evalCombatAction()
{
	switch (actionType) {
	case BASE_ATTACK:
		handleBaseAttack();
		break;
	case SPELL:
		handleSpell();
		break;
	case SKILL:
		handleSkill();
		break;
	case INVENTORY:
		handleInventory();
		break;
	case FLIGHT:
		handleFlight();
		break;
	}
}

void CombatAction::handleBaseAttack() {}

void CombatAction::handleSpell() {}

void CombatAction::handleSkill() {}

void CombatAction::handleInventory() {}

void CombatAction::handleFlight() {}