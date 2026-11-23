#pragma once
#include "application/callbacks/BattleMenuCallbacks.hpp"
#include "application/menu_entries/BattleActionMenuEntry.hpp"
#include "application/menu_entries/BattleActorMenuData.hpp"
#include "application/menu_entries/CombatantMenuEntry.hpp"
#include "game_state/ecs/components/combat/TargetScopeComponent.hpp"
#include "presentation/screens/core/Screen.hpp"

class BattleScreen : public Screen {
  private:
	BattleScreen();
	std::vector<CombatantMenuEntry> playerParty;
	std::vector<CombatantMenuEntry> enemies;
	std::size_t currentPartyMemberIndex = 0;

	BattleActorMenuData battleActorMenuData;

	BattleMenuCallbacks callbacks;

	std::unique_ptr<Widget> createMainBattleMenu();

	std::unique_ptr<Widget> createSubMenu(const std::vector<BattleActionMenuEntry> &entries);

	std::unique_ptr<Widget>
	createTargetMenu(TargetScope targetScope,
	                 const std::function<void(std::vector<Engine::ECS::Entity>)> &onTargetSelected);

	void selectAction(const BattleActionMenuEntry &action);
	void showMain();
	void showSpellMenu();
	void showSkillMenu();
	void showTargetMenu(TargetScope targetScope,
	                    const std::function<void(std::vector<Engine::ECS::Entity>)> &onTargetSelected);

	[[nodiscard]] std::optional<std::string> getTargetLabel(TargetScope scope,
	                                                        const std::vector<Engine::ECS::Entity> &targets) const;

  public:
	static BattleScreen &getInstance();
	void setCallbacks(BattleMenuCallbacks newCallbacks);
	void setBattleInfo(std::vector<CombatantMenuEntry> newPlayerParty, std::vector<CombatantMenuEntry> newEnemies,
	                   std::size_t partyMemberIndex);
	void setBattleActorMenuData(BattleActorMenuData data);

	void showBattleDialog(const std::string &battleDialogMessage, std::function<void()> onFinished);
	void playBattleAnimation();

	BattleScreen(const BattleScreen &) = delete;
	BattleScreen &operator=(const BattleScreen &) = delete;
};