#include "ecs/overworld/systems/OverworldSystemManager.hpp"

#include "ecs/overworld/systems/AnimationSystem.hpp"
#include "ecs/overworld/systems/InputSystem.hpp"
#include "ecs/overworld/systems/InteractionSystem.hpp"
#include "ecs/overworld/systems/MovementSystem.hpp"

OverworldSystemManager::OverworldSystemManager(OverworldState& overworldState) {
    RegisterSystem<InputSystem>();
    RegisterSystem<InteractionSystem>(overworldState);
    RegisterSystem<AnimationSystem>();
    RegisterSystem<MovementSystem>();
}
