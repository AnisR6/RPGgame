#include "CombatAction.hpp"
#include "Ability/Combatable.hpp"

bool CombatAction::operator<(const CombatAction& other) const noexcept
{
	return actor->getStats().speed < other.actor->getStats().speed;
}