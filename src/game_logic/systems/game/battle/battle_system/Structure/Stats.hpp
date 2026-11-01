#pragma once

struct Stats {
	int maxHP;
	int maxMP;
	int currHP;
	int currMP;
	int attack;
	int defense;
	int magic;
	int resistance;
	int skill;
	int speed;

	Stats(const int maxHP, const int maxMP, const int currHP, const int currMP, const int attack, const int defense,
	      const int magic, const int resistence, const int skill, const int speed)
	    : maxHP(maxHP), maxMP(maxMP), currHP(currHP), currMP(currMP), attack(attack), defense(defense), magic(magic),
	      resistance(resistence), skill(skill), speed(speed)
	{
	}
};