#include "application/overworld/controllers/OverworldController.hpp"

#include "presentation/overworld/screens/OverworldScreen.hpp"

OverworldController::OverworldController(OverworldMessageBus& messageBus, OverworldScreen& screen,
                                         Engine::ECS::World& world)
    : overworldMessageBus(messageBus),
      screen(screen),
      world(world) {
    messageBus.Subscribe<OverworldInfoResponse>(
        [this](const OverworldInfoResponse& response) { UpdateOverworldInfo(response); });
}

void OverworldController::RequestOverworldInfo() const {
    overworldMessageBus.Publish(OverworldInfoRequest{});
}

void OverworldController::UpdateOverworldInfo(const OverworldInfoResponse&) {
    screen.SetWorld(world);
}
