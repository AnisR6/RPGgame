
#pragma once

#include <SFML/Graphics/Font.hpp>

#include <memory>
#include <string>
#include <unordered_map>

enum class FontId { Geneva };

class FontManager {
  private:
	std::unordered_map<FontId, std::unique_ptr<sf::Font>> fonts;

  public:
	FontManager() = default;
	~FontManager() = default;

	FontManager(const FontManager &) = delete;
	FontManager &operator=(const FontManager &) = delete;

	void loadFont(FontId id, const std::string &path);

	[[nodiscard]] const sf::Font &getFont(FontId id) const;

	[[nodiscard]] bool hasFont(FontId id) const noexcept;
};
