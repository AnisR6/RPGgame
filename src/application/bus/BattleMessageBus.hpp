#pragma once
#include "application/messages/BattleMessage.hpp"

#include <functional>
#include <vector>

class BattleMessageBus {
  private:
	std::vector<std::function<void(const BattleMessage &)>> subscribers;

  public:
	template <typename T>
	void subscribe(std::function<void(const T &)> callback)
	{
		subscribers.emplace_back([callback = std::move(callback)](const BattleMessage &message) {
			if (const auto *typedMessage = std::get_if<T>(&message)) {
				callback(*typedMessage);
			}
		});
	}

	void publish(const BattleMessage &message) const
	{
		for (const auto &subscriber : subscribers) {
			subscriber(message);
		}
	}

	void clear() { subscribers.clear(); }
};