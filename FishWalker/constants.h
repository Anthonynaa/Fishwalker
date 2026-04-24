#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr int CELL_SIZE = 50;
constexpr int MAP_WIDTH = 30;
constexpr int MAP_HEIGHT = 20;
constexpr int RANDOM_WALL_COUNT = 70;
constexpr int MONSTER_COUNT = 4;
constexpr int CHEST_COUNT = 3;

constexpr int HERO_ID = 1;
constexpr int HERO_START_HP = 100;
constexpr int HERO_MAX_HP = 100;
constexpr int HERO_ATK = 10;
constexpr int HERO_ACC = 50;
constexpr int HERO_AGL = 20;
constexpr int HERO_RES = 10;
constexpr int HERO_CRIT = 5;
constexpr int HERO_INF = 0;

constexpr int NORMAL_MONSTER_ID = 1;
constexpr int NORMAL_MONSTER_HP = 20;
constexpr int NORMAL_MONSTER_MAX_HP = 20;
constexpr int NORMAL_MONSTER_ATK = 10;
constexpr int NORMAL_MONSTER_ACC = 50;
constexpr int NORMAL_MONSTER_AGL = 20;
constexpr int NORMAL_MONSTER_RES = 10;
constexpr int NORMAL_MONSTER_CRIT = 5;
constexpr int NORMAL_MONSTER_INF = 0;

constexpr int INFECTED_MONSTER_ID = 2;
constexpr int INFECTED_MONSTER_HP = 30;
constexpr int INFECTED_MONSTER_MAX_HP = 30;
constexpr int INFECTED_MONSTER_ATK = 15;
constexpr int INFECTED_MONSTER_INFECTION_POWER = 20;
constexpr int INFECTED_MONSTER_ACC = 50;
constexpr int INFECTED_MONSTER_AGL = 20;
constexpr int INFECTED_MONSTER_RES = 10;
constexpr int INFECTED_MONSTER_CRIT = 5;
constexpr int INFECTED_MONSTER_INF = 0;

constexpr int ITEM_TYPES_COUNT = 4;
constexpr int HEAL_POTION_AMOUNT = 20;
constexpr int ANTIDOTE_AMOUNT = 30;
constexpr int ARMOR_DEF = 5;
constexpr int WEAPON_DAMAGE = 15;

constexpr int STANDARD_MULTIPLIER = 1;
constexpr int BOOSTED_MULTIPLIER = 2;

constexpr float CAMERA_SPEED = 0.1f;
constexpr float CHEST_MESSAGE_TIMER = 2.5f;

constexpr float DEFAULT_BAR_WIDTH = 800.f;
constexpr float DEFAULT_BAR_HEIGHT = 80.f;

constexpr float DEFAULT_BAR_SPEED = 100.f;
constexpr float DEFAULT_TIME_LIMIT = 1.5f;

constexpr float DEFAULT_MARKER_SIZE = 40.f;

constexpr float DEFAULT_TARGET_SIZE = 120.f;
constexpr float DEFAULT_PERFECT_ZONE = 80.f;
constexpr float DEFAULT_GOOD_ZONE = 120.f;

constexpr float ATTACK_BAR_SPEED = 400.f;
constexpr float ATTACK_TARGET_WIDTH = 120.f;
constexpr float ATTACK_TIME_LIMIT = 2.0f;
constexpr float ATTACK_PERFECT_ZONE = 80.f;
constexpr float ATTACK_GOOD_ZONE = 120.f;

constexpr float CHEST_BAR_SPEED = 300.f;
constexpr float CHEST_TARGET_WIDTH = 70.f;
constexpr float CHEST_TIME_LIMIT = 3.0f;
constexpr float CHEST_PERFECT_ZONE = 40.f;
constexpr float CHEST_GOOD_ZONE = 70.f;

#endif
