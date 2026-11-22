#pragma once

#include "EventResult.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <utility>
#include <vector>

class Widget {
  protected:
	std::vector<std::unique_ptr<Widget>> children;

  public:
	explicit Widget(std::vector<std::unique_ptr<Widget>> incomingChildren = {}) : children(std::move(incomingChildren))
	{
	}

	virtual ~Widget() = default;

	void addChild(std::unique_ptr<Widget> child) { children.push_back(std::move(child)); }

	[[nodiscard]] const std::vector<std::unique_ptr<Widget>> &getChildren() const { return children; }

	[[nodiscard]] virtual sf::Vector2f getSize() const = 0;

	virtual void draw(sf::RenderTarget &target, sf::Vector2f position) = 0;

	virtual EventResult handleEvent(const sf::Event &event) { return {}; }
};

template <typename... Types>
std::vector<std::unique_ptr<Widget>> children(Types... values)
{
	std::vector<std::unique_ptr<Widget>> result;

	(result.push_back(std::move(values)), ...);

	return result;
}
