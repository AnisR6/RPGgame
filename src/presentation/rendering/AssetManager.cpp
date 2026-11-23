#include "presentation/rendering/AssetManager.hpp"

#include <filesystem>
#include <iostream>

const sf::Texture* AssetManager::loadTexture(const std::string& path)
{
	const auto existing = textures.find(path);
	if (existing != textures.end()) {
		return &existing->second;
	}

	if (!std::filesystem::exists(path)) {
		std::cout << "Could not find " << path << "\n";
		return nullptr;
	}

	sf::Texture& texture = textures[path];
	if (!texture.loadFromFile(path)) {
		std::cout << "Could not load " << path << "\n";
	}
	return &texture;
}
