
#pragma once

#include "presentation/screens/core/Screen.hpp"

class LoadMenuScreen : public Screen {
  private:
	LoadMenuScreen();

  public:
	static LoadMenuScreen &getInstance();

	LoadMenuScreen(const LoadMenuScreen &) = delete;
	LoadMenuScreen &operator=(const LoadMenuScreen &) = delete;
};
