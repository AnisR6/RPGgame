#pragma once

#include "application/shared/bus/MessageBus.hpp"
#include "application/combat/messages/BattleMessage.hpp"

using BattleMessageBus = MessageBus<BattleMessage>;
