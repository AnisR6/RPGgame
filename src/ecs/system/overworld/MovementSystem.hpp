#pragma once

#include "ecs/system/ISystem.hpp"

class TileMap;

namespace Engine::ECS {

    // -----------------------------------------------------------------------------
    // Movement System
    // -----------------------------------------------------------------------------
    class MovementSystem : public ISystem {
        public:
            // -------------------------------------------------------------------------
            // Construction
            // -------------------------------------------------------------------------
            MovementSystem(const TileMap& tileMap);

            // -------------------------------------------------------------------------
            // System Update
            // -------------------------------------------------------------------------
            void Update(SystemContext& context, float deltaTime) override;

        private:
            // -------------------------------------------------------------------------
            // Dependencies
            // -------------------------------------------------------------------------
            const TileMap& tileMap;
    };

} // namespace Engine::ECS