#pragma once
#include "application/events/BattleModeEvent.hpp"

#include <vector>

class BattleModeEventBus {
  private:
	std::vector<BattleModeEvent> events;

  public:
	void publish(const BattleModeEvent &event);
	const std::vector<BattleModeEvent> &getPendingEvents() const;
	void clear();
};