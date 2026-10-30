#include "CombatAction.hpp"

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

void CombatAction::handleBaseAttack() const
{
	// TODO: damage dealt should not only depend on the atk of the actor but also equipment
	const int actor_atk = actor->getStats().attack;
	for (const auto &target : targets) {
		const int target_def = target->getStats().defense;
		const int damage = compute_physical_damage(actor_atk, target_def);
		target->takeDamage(damage);
	}
}

int CombatAction::compute_physical_damage(const int actor_atk, const int target_def)
{
	return std::min(0, actor_atk - target_def);
}

void CombatAction::handleSpell() {}

void CombatAction::handleSkill() {}

void CombatAction::handleInventory() {}

void CombatAction::handleFlight() {}
