#pragma once

enum class TargetScope { SingleEnemy, AllEnemies, SingleAlly, AllAllies, Self };

struct TargetScopeComponent {
        TargetScope targetScope;
};
