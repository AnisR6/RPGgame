#pragma once

#include "application/shared/bus/MessageBus.hpp"
#include "application/overworld/messages/OverworldMessage.hpp"

using OverworldMessageBus = MessageBus<OverworldMessage>;
