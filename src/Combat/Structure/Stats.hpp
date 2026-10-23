#pragma once

struct Stats {
	unsigned maxHP;
	unsigned maxMP;
    unsigned currHP;
    unsigned currMP;
    unsigned attack;
    unsigned defense;
    unsigned magic;
    unsigned resistence;
    unsigned skill;
    unsigned speed;

    Stats(unsigned maxHP, unsigned maxMP, unsigned currHP, unsigned currMP, unsigned attack, unsigned defense, unsigned magic, unsigned resistence, unsigned skill, unsigned speed)
        : maxHP(maxHP), maxMP(maxMP), currHP(currHP), currMP(currMP), attack(attack), defense(defense), magic(magic), resistence(resistence), skill(skill), speed(speed) {}
};