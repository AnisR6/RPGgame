#pragma once
#include <set>

#include "Combatable.hpp"
#include "Playable.hpp"
#include "Enemyable.hpp"
#include "Combat/CombatAction.hpp"

using PlayerParty = std::vector<std::unique_ptr<Playable>>;
using EnemyParty = std::vector<std::unique_ptr<Enemyable>>;



class BattleMode {
    private:
        PlayerParty playerParty;
        EnemyParty enemyParty;
        std::set<CombatAction> combatActions;

    public:
        BattleMode(PlayerParty playerParty, EnemyParty enemyParty);
        void executeTurn();
};