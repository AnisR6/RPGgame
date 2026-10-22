#include "BattleMode.hpp"

BattleMode::BattleMode(PlayerParty playerParty, EnemyParty enemyParty) : playerParty(std::move(playerParty)), enemyParty(std::move(enemyParty)) {}



void BattleMode::executeTurn() {

    // bad design, the players and enemies can share this via Combatable
    for (auto& player : playerParty) {
        CombatAction playerAction = player->chooseAction();
        combatActions.insert(playerAction);
    }

    for (auto& enemy : enemyParty) {
        CombatAction enemyAction = enemy->chooseAction();
        combatActions.insert(combatActions);
    }
}
