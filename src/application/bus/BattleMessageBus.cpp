#include "BattleMessageBus.hpp"

template <typename T>
void BattleMessageBus::subscribe(std::function<void(const T &)> callback)
{
	subscribers.emplace_back([callback = std::move(callback)](const BattleMessage &message) {
		if (const auto *typedMessage = std::get_if<T>(&message)) {
			callback(*typedMessage);
		}
	});
}

void BattleMessageBus::publish(const BattleMessage &message) const
{
	for (const auto &subscriber : subscribers) {
		subscriber(message);
	}
}

void BattleMessageBus::clear()
{
	subscribers.clear();
}