#include "ecs/core/storage/SparseSet.hpp"

namespace Engine::ECS {
    bool SparseSet::Contains(Index entityIndex) const noexcept {
        return GetPackedIndex(entityIndex) != InvalidIndex;
    }

    bool SparseSet::Insert(Index entityIndex) {
        if (entityIndex == InvalidIndex || GetPackedIndex(entityIndex) != InvalidIndex)
            return false;

        if (packed.size() >= InvalidIndex)
            return false;

        if (entityIndex >= sparse.size())
            sparse.resize(static_cast<std::size_t>(entityIndex) + 1, InvalidIndex);

        const Index packedIndex = static_cast<Index>(packed.size());

        packed.push_back(entityIndex);
        sparse[entityIndex] = packedIndex;

        return true;
    }

    bool SparseSet::Remove(Index entityIndex) noexcept {
        const Index packedIndex = GetPackedIndex(entityIndex);

        if (packedIndex == InvalidIndex)
            return false;

        const Index lastIndex = static_cast<Index>(packed.size() - 1);

        if (packedIndex != lastIndex) {
            const Index lastEntity = packed[lastIndex];

            packed[packedIndex] = lastEntity;
            sparse[lastEntity] = packedIndex;
        }

        packed.pop_back();
        sparse[entityIndex] = InvalidIndex;

        return true;
    }

    void SparseSet::Reserve(std::size_t capacity) {
        sparse.reserve(capacity);
        packed.reserve(capacity);
    }

    SparseSet::Index SparseSet::GetPackedIndex(Index entityIndex) const noexcept {
        if (entityIndex >= sparse.size())
            return InvalidIndex;

        const Index packedIndex = sparse[entityIndex];

        if (packedIndex == InvalidIndex || packedIndex >= packed.size() ||
            packed[packedIndex] != entityIndex)
            return InvalidIndex;

        return packedIndex;
    }

    SparseSet::Index SparseSet::GetPackedEntity(std::size_t packedIndex) const noexcept {
        if (packedIndex >= packed.size())
            return InvalidIndex;

        return packed[packedIndex];
    }

    const std::vector<SparseSet::Index>& SparseSet::GetPackedEntities() const noexcept {
        return packed;
    }

    std::size_t SparseSet::GetSize() const noexcept {
        return packed.size();
    }

    bool SparseSet::IsEmpty() const noexcept {
        return packed.empty();
    }
}
