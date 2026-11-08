#pragma once
#include "TargetScope.hpp"
#include <string>

struct CombatSkill {
	int skillId;
	std::string skillName;
	TargetScope targetScope;
	int power;
};