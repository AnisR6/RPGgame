#include "presentation/widgets/menu/MenuRow.hpp"

MenuRow::MenuRow(std::vector<std::unique_ptr<Widget>> incomingChildren, float spacing)
    : MenuLayout(std::move(incomingChildren), spacing, MenuOrientation::Row)
{
}
