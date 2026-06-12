#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr int HERO_ID = 1;
constexpr int HERO_START_HP = 100;
constexpr int HERO_MAX_HP = 100;
constexpr int HERO_ATK = 10;
constexpr int HERO_INF = 0;

constexpr int NORMAL_MONSTER_ID = 1;
constexpr int NORMAL_MONSTER_HP = 20;
constexpr int NORMAL_MONSTER_MAX_HP = 20;
constexpr int NORMAL_MONSTER_ATK = 10;
constexpr int NORMAL_MONSTER_INF = 0;

constexpr int INFECTED_MONSTER_ID = 2;
constexpr int INFECTED_MONSTER_HP = 30;
constexpr int INFECTED_MONSTER_MAX_HP = 30;
constexpr int INFECTED_MONSTER_ATK = 15;
constexpr int INFECTED_MONSTER_INFECTION_POWER = 20;
constexpr int INFECTED_MONSTER_INF = 0;

constexpr int STANDARD_MULTIPLIER = 1;
constexpr int BOOSTED_MULTIPLIER = 2;

constexpr int SPEED_BONUS_BASE = 33;
constexpr int DAMAGE_PERCENT_BASE = 100;
constexpr int DAMAGE_PERCENT_BONUS = 25;
constexpr int HEAVY_ATTACK_COOLDOWN = 3;
constexpr int WEAPON_SLOTS = 3;
constexpr int ARMOR_SLOTS = 3;

constexpr int CELL_SIZE = 3;
constexpr int FIXED_CELLS_COUNT = 16;
constexpr int MAX_CYCLES = 5;
constexpr int BASE_MOVE_INTERVAL_MS = 100;
constexpr int LOOP_DELAY_MS = 20;

const char NORMAL_CHAR = '#';
const char TARGET_CROSS = '+';
const char TARGET_BG = '.';
const char FRAME_TOP = '-';
const char FRAME_BOTTOM = '-';
const char FRAME_LEFT = '|';
const char FRAME_RIGHT = '|';
const char FRAME_CORNER = '+';

#endif
