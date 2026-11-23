#pragma once

enum class TargetScope { SINGLE_ENEMY, ALL_ENEMIES, SINGLE_ALLY, ALL_ALLIES, SELF };

struct TargetScopeComponent {
	TargetScope targetScope;
};
