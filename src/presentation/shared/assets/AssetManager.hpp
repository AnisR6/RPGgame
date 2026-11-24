#pragma once

#include "ecs/core/entity/Entity.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <string>
#include <unordered_map>

namespace Engine::ECS {
    class World;
}

enum class FontId { Geneva };

class AssetManager {
    public:
        const sf::Texture* LoadTexture(const std::string& path);

        void LoadFont(FontId id, const std::string& path);

        const sf::Font& GetFont(FontId id) const;
        bool HasFont(FontId id) const noexcept;

        Engine::ECS::Entity LoadTileMap(const std::string& mapFilePath,
                                        const std::string& tilesetFolderPath,
                                        Engine::ECS::World& world);

    private:
        std::unordered_map<std::string, sf::Texture> textures;
        std::unordered_map<FontId, std::unique_ptr<sf::Font>> fonts;
};
