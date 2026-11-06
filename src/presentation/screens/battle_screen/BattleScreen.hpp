#pragma once
#include "presentation/screens/core/Screen.hpp"

class BattleScreen : public Screen {
  private:
	BattleScreen();
	std::unique_ptr<Widget> createMainBattleMenu();
	template <typename T>
	std::unique_ptr<Widget> createSubMenu(std::vector<T> &subMenuEntry);

	void showMain();
	template <typename T>
	void showSubMenu();

  public:
	static BattleScreen &getInstance();

	BattleScreen(const BattleScreen &) = delete;
	BattleScreen &operator=(const BattleScreen &) = delete;
};