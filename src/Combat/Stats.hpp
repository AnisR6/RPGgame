#pragma once

struct Stats {
    unsigned hp;
    unsigned mp;
    unsigned attack;
    unsigned defense;
    unsigned magic;
    unsigned resistence;
    unsigned skill;
    unsigned speed;

    Stats(unsigned hp, unsigned mp, unsigned attack, unsigned defense, unsigned magic, unsigned resistence, unsigned skill, unsigned speed)
        : hp(hp), mp(mp), attack(attack), defense(defense), magic(magic), resistence(resistence), skill(skill), speed(speed) {}
};