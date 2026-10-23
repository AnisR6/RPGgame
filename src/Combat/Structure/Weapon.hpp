#pragma once

enum WeaponType {
    SWORD,
    LANCE,
    AXE,
    STAFF
};


struct Weapon {
    WeaponType type;
    unsigned baseDamage;
    float critRate;
};