#pragma once

#include "Combat/Stats.hpp"
#include "Combat/CombatAction.hpp"

class Combatable {
    public:
        virtual Stats& getStats() const = 0;
        virtual CombatAction chooseAction() = 0;
        virtual ~Combatable() = default;

};





