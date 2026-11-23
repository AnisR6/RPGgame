#pragma once
#include "ecs/shared/components/NameComponent.hpp"
#include "ecs/shared/components/PartyMemberComponent.hpp"
#include "ecs/shared/components/SpriteComponent.hpp"
#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/world/World.hpp"

namespace Prefabs {
    struct PartyMemberBlueprint {
            NameComponent nameComponent;
            PartyMemberComponent partyMemberComponent;
            StatsComponent statsComponent;
            KnownActionsComponent knownActionsComponent;
            SpriteComponent spriteComponent;
    };

    inline Engine::ECS::Entity CreatePartyMember(Engine::ECS::World& world,
                                                 PartyMemberBlueprint&& blueprint) {
        const auto entity = world.CreateEntity();
        world.EmplaceComponent<NameComponent>(entity, std::move(blueprint.nameComponent));
        world.EmplaceComponent<PartyMemberComponent>(entity, blueprint.partyMemberComponent);
        world.EmplaceComponent<StatsComponent>(entity, blueprint.statsComponent);
        world.EmplaceComponent<KnownActionsComponent>(entity, blueprint.knownActionsComponent);
        world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));
        return entity;
    }
}
