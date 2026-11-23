#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <unordered_map>

class AssetManager {
public:
	// loads the picture once and keeps it. nullptr if the file does not exist.
	const sf::Texture* loadTexture(const std::string& path);

private:
	std::unordered_map<std::string, sf::Texture> textures;
};
