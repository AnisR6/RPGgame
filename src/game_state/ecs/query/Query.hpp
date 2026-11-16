#pragma once

#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/entity/EntityManager.hpp"
#include "game_state/ecs/storage/ComponentRegistry.hpp"

#include <cstddef>
#include <functional>
#include <optional>
#include <type_traits>
#include <vector>

namespace Engine::ECS {

// ------------------------------------------------------------
// Query Types
// ------------------------------------------------------------

template <typename... Types>
struct Including {};

template <typename... Types>
struct Excluding {};

// ------------------------------------------------------------
// Query Declaration
// ------------------------------------------------------------

template <typename IncludeSet, typename ExcludeSet = Excluding<>>
class Query;

// ------------------------------------------------------------
// Query Type Validator
// ------------------------------------------------------------

template <typename IncludeList, typename ExcludeList>
struct QueryTypeValidator;

template <typename FirstIncluded, typename... Included, typename... Excluded>
struct QueryTypeValidator<Including<FirstIncluded, Included...>, Excluding<Excluded...>> {
	static constexpr bool ValidateQueryTypes()
	{
		ValidateNoDuplicateTypes();
		ValidateNoOverlappingTypes();
		ValidateComponentTypes();

		return true;
	}

  private:
	template <typename First, typename... Rest>
	static constexpr bool HasDuplicateTypes()
	{
		if constexpr (sizeof...(Rest) == 0) {
			return false;
		} else {
			return (std::is_same_v<First, Rest> || ...) || HasDuplicateTypes<Rest...>();
		}
	}

	template <typename T>
	[[nodiscard]]
	static constexpr bool HasOverlapWithExcluded()
	{
		return (std::is_same_v<T, Excluded> || ...);
	}

	static constexpr bool HasOverlappingTypes()
	{
		return HasOverlapWithExcluded<FirstIncluded>() || (HasOverlapWithExcluded<Included>() || ...);
	}

	static constexpr void ValidateNoDuplicateTypes()
	{
		static_assert(!HasDuplicateTypes<FirstIncluded, Included...>(),
		              "Included component types must not contain duplicates.");

		if constexpr (sizeof...(Excluded) >= 2) {
			static_assert(!HasDuplicateTypes<Excluded...>(), "Excluded component types must not contain duplicates.");
		}
	}

	static constexpr void ValidateNoOverlappingTypes()
	{
		static_assert(!HasOverlappingTypes(),
		              "A component type must not appear in both the included and excluded sets.");
	}

	static constexpr void ValidateComponentTypes()
	{
		ComponentRegistry::ValidateComponentType<FirstIncluded>();

		(ComponentRegistry::ValidateComponentType<Included>(), ...);
		(ComponentRegistry::ValidateComponentType<Excluded>(), ...);
	}
};

// ------------------------------------------------------------
// Query Implementation
// ------------------------------------------------------------

template <typename FirstIncluded, typename... Included, typename... Excluded>
class Query<Including<FirstIncluded, Included...>, Excluding<Excluded...>> {
  public:
	// ------------------------------------------------------------
	// Types
	// ------------------------------------------------------------

	using PrimaryComponent = FirstIncluded;
	using PrimaryPool = ComponentPool<PrimaryComponent>;

	// ------------------------------------------------------------
	// Construction
	// ------------------------------------------------------------

	Query(EntityManager &entityManager, ComponentRegistry &componentRegistry)
	    : entityManager(entityManager), componentRegistry(componentRegistry)
	{
	}

  private:
	// ------------------------------------------------------------
	// Validation
	// ------------------------------------------------------------

	static_assert(
	    QueryTypeValidator<Including<FirstIncluded, Included...>, Excluding<Excluded...>>::ValidateQueryTypes());

  public:
	// ------------------------------------------------------------
	// Iteration
	// ------------------------------------------------------------

	template <typename Function>
	void ForEach(Function &&function) const
	{
		const std::vector<Entity> entities = SnapshotPreIterationEntities();

		for (const Entity entity : entities) {
			if (!entityManager.IsAlive(entity) || entityManager.IsPendingDestroy(entity))
				continue;

			const Entity::Index entityIndex = entity.GetIndex();

			if (!HasAllIncluded<FirstIncluded, Included...>(entityIndex))
				continue;

			if (HasAnyExcluded<Excluded...>(entityIndex))
				continue;

			std::invoke(function, entity, componentRegistry.GetPool<FirstIncluded>()->GetComponent(entityIndex),
			            componentRegistry.GetPool<Included>()->GetComponent(entityIndex)...);
		}
	}

	// ------------------------------------------------------------
	// Accessors
	// ------------------------------------------------------------

	[[nodiscard]]
	std::size_t Count() const
	{
		std::size_t matchingEntityCount = 0;

		ForEach([&matchingEntityCount](auto &&...) { ++matchingEntityCount; });

		return matchingEntityCount;
	}

	[[nodiscard]]
	bool IsEmpty() const
	{
		return Count() == 0;
	}

	[[nodiscard]]
	std::optional<Entity> TryGetSingleEntity() const
	{
		std::optional<Entity> result;
		std::size_t count = 0;
		ForEach([&](Entity entity, auto &&...) {
			count++;
			result = entity;
		});

		return count == 1 ? result : std::nullopt;
	}

  private:
	// ------------------------------------------------------------
	// Matching Helpers
	// ------------------------------------------------------------

	template <typename T>
	bool HasComponent(Entity::Index entityIndex) const
	{
		ComponentPool<T> *componentPool = componentRegistry.GetPool<T>();

		return componentPool != nullptr && componentPool->ContainsComponent(entityIndex);
	}

	template <typename... Types>
	bool HasAllIncluded(Entity::Index entityIndex) const
	{
		return (HasComponent<Types>(entityIndex) && ...);
	}

	template <typename... Types>
	bool HasAnyExcluded(Entity::Index entityIndex) const
	{
		return (HasComponent<Types>(entityIndex) || ...);
	}

	// ------------------------------------------------------------
	// Snapshot
	// ------------------------------------------------------------

	[[nodiscard]]
	ComponentPool<FirstIncluded> *GetPrimaryPool() const;

	[[nodiscard]]
	bool HasPrimaryPool(const ComponentPool<FirstIncluded> *primaryPool) const noexcept;

	void ReserveSnapshot(std::vector<Entity> &entities, const ComponentPool<FirstIncluded> &primaryPool) const;

	[[nodiscard]]
	Entity GetEntityAtPackedIndex(const ComponentPool<FirstIncluded> &primaryPool, std::size_t packedIndex) const;

	[[nodiscard]]
	std::vector<Entity> SnapshotPreIterationEntities() const;

	// ------------------------------------------------------------
	// Storage
	// ------------------------------------------------------------

	EntityManager &entityManager;
	ComponentRegistry &componentRegistry;
};

// ------------------------------------------------------------
// GetPrimaryPool
// ------------------------------------------------------------

template <typename FirstIncluded, typename... Included, typename... Excluded>
ComponentPool<FirstIncluded> *
Query<Including<FirstIncluded, Included...>, Excluding<Excluded...>>::GetPrimaryPool() const
{
	return componentRegistry.GetPool<FirstIncluded>();
}

// ------------------------------------------------------------
// HasPrimaryPool
// ------------------------------------------------------------

template <typename FirstIncluded, typename... Included, typename... Excluded>
bool Query<Including<FirstIncluded, Included...>, Excluding<Excluded...>>::HasPrimaryPool(
    const ComponentPool<FirstIncluded> *primaryPool) const noexcept
{
	return primaryPool != nullptr;
}

// ------------------------------------------------------------
// ReserveSnapshot
// ------------------------------------------------------------

template <typename FirstIncluded, typename... Included, typename... Excluded>
void Query<Including<FirstIncluded, Included...>, Excluding<Excluded...>>::ReserveSnapshot(
    std::vector<Entity> &entities, const ComponentPool<FirstIncluded> &primaryPool) const
{
	entities.reserve(primaryPool.GetSize());
}

// ------------------------------------------------------------
// GetEntityAtPackedIndex
// ------------------------------------------------------------

template <typename FirstIncluded, typename... Included, typename... Excluded>
Entity Query<Including<FirstIncluded, Included...>, Excluding<Excluded...>>::GetEntityAtPackedIndex(
    const ComponentPool<FirstIncluded> &primaryPool, std::size_t packedIndex) const
{
	const Entity::Index entityIndex = primaryPool.GetEntityAt(packedIndex);

	return entityManager.GetEntity(entityIndex);
}

// ------------------------------------------------------------
// SnapshotPreIterationEntities
// ------------------------------------------------------------

template <typename FirstIncluded, typename... Included, typename... Excluded>
std::vector<Entity>
Query<Including<FirstIncluded, Included...>, Excluding<Excluded...>>::SnapshotPreIterationEntities() const
{
	std::vector<Entity> entities;

	ComponentPool<FirstIncluded> *primaryPool = GetPrimaryPool();

	if (!HasPrimaryPool(primaryPool))
		return entities;

	ReserveSnapshot(entities, *primaryPool);

	for (std::size_t packedIndex = 0; packedIndex < primaryPool->GetSize(); ++packedIndex)
		entities.push_back(GetEntityAtPackedIndex(*primaryPool, packedIndex));

	return entities;
}

} // namespace Engine::ECS
