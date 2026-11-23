#pragma once

// TODO: implement behavior of different profiles
enum class EnemyAIProfile { AGGRESSIVE_MELEE, SUPPORT, DEBUFFER };

struct EnemyAIComponent {
	EnemyAIProfile profile;
};
