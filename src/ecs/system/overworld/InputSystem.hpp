#pragma once

#include "ecs/system/ISystem.hpp"

namespace Engine::ECS {

// -----------------------------------------------------------------------------
// Input System
// -----------------------------------------------------------------------------
class InputSystem : public ISystem {
public:

    // -------------------------------------------------------------------------
    // System Update
    // -------------------------------------------------------------------------
    void Update(SystemContext& context, float deltaTime) override;

private:

    // -------------------------------------------------------------------------
    // Input State
    // -------------------------------------------------------------------------
    bool wasInteractKeyDown = false;
};

} // namespace Engine::ECS