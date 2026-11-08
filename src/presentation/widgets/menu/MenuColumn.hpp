#pragma once

#include "presentation/widgets/menu/MenuLayout.hpp"

#include <memory>
#include <utility>
#include <vector>

class MenuColumn : public MenuLayout {
  public:
	explicit MenuColumn(std::vector<std::unique_ptr<Widget>> incomingChildren = {}, float spacing = 12.f);
};

inline std::unique_ptr<MenuColumn> column(std::vector<std::unique_ptr<Widget>> items)
{
	return std::make_unique<MenuColumn>(std::move(items));
}
