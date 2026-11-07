#pragma once
#include "application/messages/BattleMessage.hpp"

#include <functional>
#include <vector>

class BattleMessageBus {
  private:
	std::vector<std::function<void(const BattleMessage &)>> subscribers;

  public:
	void publish(const BattleMessage &event) const;
	template <typename T>
	void subscribe(std::function<void(const T &)> callback);

	void clear();
};