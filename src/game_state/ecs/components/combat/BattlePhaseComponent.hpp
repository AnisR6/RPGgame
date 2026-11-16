#pragma once

enum class BattlePhase { PlayerTurn, EnemyAITurn, ExecuteTurn, WinBattle, LoseBattle };

struct BattlePhaseComponent {
	BattlePhase currentPhase = BattlePhase::PlayerTurn;
};
