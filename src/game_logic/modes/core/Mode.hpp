#pragma once
#include "application/events/core/MessageEvent.hpp"

#include <SFML/Window/Event.hpp>
#include <optional>

class Mode {
  public:
	virtual ~Mode() = default;
	virtual std::optional<MessageEvent> translate(const sf::Event &event) = 0;
};