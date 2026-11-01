#include "game_logic/systems/game/battle/battle_system/BattleSystem.hpp"
#include "game_logic/systems/game/battle/battle_system/Enemy/Enemy.hpp"
#include "game_logic/systems/game/battle/battle_system/Party/PartyMember.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Stats.hpp"
#include "game_state/ecs/capabilities/Combatable.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>

namespace {
class TestHero : public PartyMember {
  public:
	explicit TestHero(const Stats &stats) : PartyMember{stats, {}, {}} {}
};
} // namespace

namespace {
class TestEnemy : public Enemy {
  public:
	explicit TestEnemy(const Stats &stats) : Enemy(stats, {}, {}) {}
};
} // namespace

TEST_CASE("Speed-based CombatAction sorting", "[combat]")
{
	constexpr Stats heroStats{.speed = 15};
	constexpr Stats enemyStats{.speed = 20};

	TestHero hero{heroStats};
	TestEnemy enemy{enemyStats};

	SECTION("Actions with real combatants sort correctly by speed")
	{

		std::vector<Combatable *> combatants = {&hero, &enemy};

		const CombatAction heroAction(&hero, ActionType::BASE_ATTACK, {&enemy});
		const CombatAction enemyAction(&enemy, ActionType::BASE_ATTACK, {&hero});

		std::vector<CombatAction> actions = {heroAction, enemyAction};

		std::ranges::sort(actions, std::greater<CombatAction>{});

		REQUIRE(actions[0].actor == &enemy);
		REQUIRE(actions[1].actor == &hero);
	}
}