#pragma once

#include "ecs/overworld/components/MapComponent.hpp"
#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/query/Query.hpp"

namespace MapLookup {
    template <typename Context> const MapComponent* FindMap(Context& context) {
        const MapComponent* map = nullptr;

        auto maps = context.template CreateQuery<Engine::ECS::Including<MapComponent>>();
        maps.ForEach([&](Engine::ECS::Entity, MapComponent& mapComponent) { map = &mapComponent; });

        return map;
    }
}
