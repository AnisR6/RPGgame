#pragma once

#include "ecs/system/ISystem.hpp"

struct TileMapData;

namespace Engine::ECS {

    // -----------------------------------------------------------------------------
    // Movement System
    // -----------------------------------------------------------------------------
    class MovementSystem : public ISystem {
        public:
            // -------------------------------------------------------------------------
            // Construction
            // -------------------------------------------------------------------------
            MovementSystem(const TileMapData& tileMap);

            // -------------------------------------------------------------------------
            // System Update
            // -------------------------------------------------------------------------
            void Update(SystemContext& context, float deltaTime) override;

        private:
            // -------------------------------------------------------------------------
            // Dependencies
            // -------------------------------------------------------------------------
            const TileMapData& tileMap;
    };

} // namespace Engine::ECS