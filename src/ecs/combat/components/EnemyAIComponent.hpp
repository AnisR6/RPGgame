#pragma once

enum class EnemyAIProfile { AggressiveMelee, Support, Debuffer };

struct EnemyAIComponent {
        EnemyAIProfile profile;
};
