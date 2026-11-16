#include <algorithm>
#include <catch2/catch_test_macros.hpp>

// TODO: Migrate Test after migrating logic is finished
/*
namespace {
class TestHero : public PartyMember {
public:
  explicit TestHero(const StatsComponent &stats) : PartyMember{stats, {}, {}} {}
};
} // namespace

namespace {
class TestEnemy : public Enemy {
public:
  explicit TestEnemy(const StatsComponent &stats) : Enemy(stats, {}, {}) {}
};
} // namespace

TEST_CASE("Speed-based CombatAction sorting", "[combat]") {
  constexpr StatsComponent heroStats{.speed = 15};
  constexpr StatsComponent enemyStats{.speed = 20};

  TestHero hero{heroStats};
  TestEnemy enemy{enemyStats};

  SECTION("Actions with real combatants sort correctly by speed") {

    std::vector<Combatable *> combatants = {&hero, &enemy};

    const CombatAction heroAction(&hero, ActionType::BASE_ATTACK, {&enemy});
    const CombatAction enemyAction(&enemy, ActionType::BASE_ATTACK, {&hero});

    std::vector<CombatAction> actions = {heroAction, enemyAction};

    std::ranges::sort(actions, std::greater<CombatAction>{});

    REQUIRE(actions[0].actor == &enemy);
    REQUIRE(actions[1].actor == &hero);
  }
}

TEST_CASE("CombatAction healing", "[combat]") {
  constexpr StatsComponent heroStats{.speed = 15};

  constexpr StatsComponent enemyStats{.speed = 20};

  TestHero hero{heroStats};
  TestEnemy enemy{enemyStats};

  SECTION("Health spell restores HP") {
    Spell healingSpell{
        .spellType = SpellType::HEALTH, .consumedMP = 10, .power = 30};
    enemy.takeDamage(50);

    CombatAction action =
        CombatAction::createSpellAction(&hero, {&enemy}, healingSpell);

    action.evalCombatAction();

    REQUIRE(enemy.getStats().currHP == 80);
    REQUIRE(hero.getStats().currMP == heroStats.currMP - 10);
  }
}
*/
