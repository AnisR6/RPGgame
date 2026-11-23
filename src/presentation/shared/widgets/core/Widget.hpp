#pragma once

#include "presentation/shared/widgets/core/EventResult.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <utility>
#include <vector>

class AssetManager;

class Widget {
    protected:
        std::vector<std::unique_ptr<Widget>> children;

    public:
        explicit Widget(std::vector<std::unique_ptr<Widget>> incomingChildren = {})
            : children(std::move(incomingChildren)) {}

        virtual ~Widget() = default;

        void AddChild(std::unique_ptr<Widget> child) { children.push_back(std::move(child)); }

        [[nodiscard]] const std::vector<std::unique_ptr<Widget>>& GetChildren() const {
            return children;
        }

        [[nodiscard]] virtual sf::Vector2f GetSize() const = 0;

        virtual void Draw(sf::RenderTarget& target, sf::Vector2f position) = 0;

        virtual EventResult HandleEvent(const sf::Event& event) {
            for (const auto& child : children) {
                EventResult result = child->HandleEvent(event);

                if (result.handled) {
                    return result;
                }
            }

            return {};
        }

        virtual void BindFonts(const AssetManager& assets) {
            for (const auto& child : children) {
                child->BindFonts(assets);
            }
        }
};

template <typename... Types> std::vector<std::unique_ptr<Widget>> children(Types... values) {
    std::vector<std::unique_ptr<Widget>> result;

    (result.push_back(std::move(values)), ...);

    return result;
}
