#pragma once

#include "presentation/screens/core/Screen.hpp"

class MainMenuScreen : public Screen {
  private:
	MainMenuScreen();

  public:
	static MainMenuScreen &getInstance();

	MainMenuScreen(const MainMenuScreen &) = delete;
	MainMenuScreen &operator=(const MainMenuScreen &) = delete;
};