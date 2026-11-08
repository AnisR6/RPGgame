#include "CombatAction.hpp"

bool CombatAction::operator<(const CombatAction &other) const noexcept
{
	return actor->getStats().speed < other.actor->getStats().speed;
}

bool CombatAction::operator>(const CombatAction &other) const noexcept
{
	return actor->getStats().speed > other.actor->getStats().speed;
}

template <typename F>
void CombatAction::applyToTargets(F &&func) const
{
	for (auto &target : targets) {
		func(target);
	}
}

void CombatAction::evalCombatAction()
{
	switch (actionType) {
	case ActionType::BASE_ATTACK:
		handleBaseAttack();
		break;
	case ActionType::SPELL:
		handleSpell();
		break;
	case ActionType::SKILL:
		handleSkill();
		break;
	case ActionType::INVENTORY:
		handleInventory();
		break;
	case ActionType::FLIGHT:
		handleFlight();
		break;
	}
}

void CombatAction::handleBaseAttack() const
{
	// TODO: damage dealt should not only depend on the atk of the actor but also equipment
	const int actor_atk = actor->getStats().attack;
	applyToTargets([actor_atk](Combatable *target) {
		const int damage = computePhysicalDamage(actor_atk, target->getStats().defense);
		target->takeDamage(damage);
	});
}

int CombatAction::computePhysicalDamage(const int actor_atk, const int target_def)
{
	return std::max(0, actor_atk - target_def);
}

int CombatAction::computeMagicalDamage(const int actor_mag, const int target_res)
{
	// TODO: implement weaknesses/resistances for elements
	return std::max(0, actor_mag - target_res);
}

void CombatAction::handleAttackSpell() const
{
	const int actor_mag = actor->getStats().magic;
	applyToTargets([actor_mag](Combatable *target) {
		const int damage = computeMagicalDamage(actor_mag, target->getStats().resistance);
		target->takeDamage(damage);
	});
}

void CombatAction::handleHealthSpell() const
{
	applyToTargets([this](Combatable *target) { target->heal(spell->power); });
}

void CombatAction::handleStatusSpell()
{
	// TODO: implement different status effects
}

static void reduceMp(Combatable *actor, const Spell *spell)
{
	actor->getStats().currMP = std::max(0, actor->getStats().currMP - spell->consumedMP);
}

void CombatAction::handleSpell()
{
	reduceMp(actor, spell);
	switch (spell->spellType) {
	case SpellType::HEALTH:
		return handleHealthSpell();
		break;
	case SpellType::STATUS:
		return handleStatusSpell();
		break;
	default:
		return handleAttackSpell();
	}
}

void CombatAction::handleSkill() const
{
	applyToTargets([this](Combatable *target) { target->takeDamage(skill->power); });
}

void CombatAction::handleInventory() {}

void CombatAction::handleFlight() {}
