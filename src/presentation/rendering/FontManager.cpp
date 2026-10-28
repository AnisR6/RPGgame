
#include "presentation/rendering/FontManager.hpp"

#include <stdexcept>
#include <utility>

void FontManager::loadFont(FontId id, const std::string &path)
{
	auto font = std::make_unique<sf::Font>();

	if (!font->openFromFile(path)) {
		throw std::runtime_error("FontManager: failed to load font: " + path);
	}

	fonts.insert_or_assign(id, std::move(font));
}

const sf::Font &FontManager::getFont(FontId id) const
{
	const auto it = fonts.find(id);

	if (it == fonts.end()) {
		throw std::out_of_range("FontManager: requested font has not been loaded");
	}

	return *it->second;
}

bool FontManager::hasFont(FontId id) const noexcept
{
	return fonts.find(id) != fonts.end();
}
