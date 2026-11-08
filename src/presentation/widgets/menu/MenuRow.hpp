#pragma once

#include "presentation/widgets/menu/MenuLayout.hpp"

#include <memory>
#include <utility>
#include <vector>

class MenuRow : public MenuLayout {
  public:
	explicit MenuRow(std::vector<std::unique_ptr<Widget>> incomingChildren = {}, float spacing = 12.f);
};

inline std::unique_ptr<MenuRow> row(std::vector<std::unique_ptr<Widget>> items)
{
	return std::make_unique<MenuRow>(std::move(items));
}
