#include "presentation/widgets/menu/MenuColumn.hpp"

MenuColumn::MenuColumn(std::vector<std::unique_ptr<Widget>> incomingChildren, float spacing)
    : MenuLayout(std::move(incomingChildren), spacing, MenuOrientation::Column)
{
}
