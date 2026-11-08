#include "BattleMessageBus.hpp"

template <typename T>
void BattleMessageBus::subscribe(std::function<void(const T &)> callback)
{
	subscribers.emplace_back([callback = std::move(callback)](const BattleMessage &event) {
		if (const auto *typedEvent = std::get_if<T>(&event)) {
			callback(*typedEvent);
		}
	});
}

void BattleMessageBus::publish(const BattleMessage &event) const
{
	for (const auto &subscriber : subscribers) {
		subscriber(event);
	}
}

void BattleMessageBus::clear()
{
	subscribers.clear();
}