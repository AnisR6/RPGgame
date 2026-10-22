#pragma once
#include <vector>

#include "Ability/Combatable.hpp"
#include "Combat/Stats.hpp"
#include "Combat/CombatSkill.hpp"
#include "Combat/Spell.hpp"
#include "Combat/CombatAction.hpp"


class PartyMember : public Combatable {
  private:
    Stats stats;
    std::vector<CombatSkill> combatSkills;
    std::vector<Spell> spells;

  public:
    Stats& getStats() const override;
    CombatAction chooseAction() override;
};
