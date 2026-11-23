#pragma once

#include "ecs/system/ISystem.hpp"

class TileMap;

namespace Engine::ECS {

    class MovementSystem : public ISystem {
        public:
            MovementSystem(const TileMap& tileMap);

            void Update(SystemContext& context, float deltaTime) override;

        private:
            const TileMap& tileMap; // to know where the walls are
    };

} // namespace Engine::ECS
