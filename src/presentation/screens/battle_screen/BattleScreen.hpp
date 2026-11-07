#pragma once
#include "application/battle/CombatantInfo.hpp"
#include "application/callbacks/BattleMenuCallbacks.hpp"
#include "presentation/screens/core/Screen.hpp"

class BattleScreen : public Screen {
  private:
	BattleScreen();
	std::vector<CombatantInfo> playerPartyInfo;
	std::vector<CombatantInfo> enemyPartyInfo;
	std::size_t currentPartyMemberIndex;

	BattleMenuCallbacks callbacks;

	std::unique_ptr<Widget> createMainBattleMenu();

	template <typename T, typename GetId, typename GetName>
	std::unique_ptr<Widget> createSubMenu(const std::vector<T> &subMenuEntries,
	                                      const std::function<void(int)> &onSelected, GetId getId, GetName getName);
	void showMain();
	template <typename T>
	void showSubMenu();

  public:
	static BattleScreen &getInstance();
	void setCallbacks(BattleMenuCallbacks newCallbacks);
	void setBattleInfo(const std::vector<CombatantInfo> &playerParty, const std::vector<CombatantInfo> &enemies,
	                   std::size_t partyMemberIndex);

	BattleScreen(const BattleScreen &) = delete;
	BattleScreen &operator=(const BattleScreen &) = delete;
};