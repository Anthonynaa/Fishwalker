#include "battle.h"

#include <iostream>

Battle::Battle(Hero* hero, std::vector<Monster*> enemies)
    : hero(hero), enemies(enemies) {}

Battle::~Battle() {}

bool Battle::isBattleOver() const {
  if (!hero->isAlive()) return true;
  for (auto* m : enemies)
    if (m->isAlive()) return false;
  return true;
}

int Battle::countAliveEnemies() const {
  int cnt = 0;
  for (auto* m : enemies)
    if (m->isAlive()) cnt++;
  return cnt;
}

Hero& Battle::getHero() { return *hero; }
Monster& Battle::getMonster() { return *enemies[0]; }

void Battle::heroAttack() {
  if (!isBattleOver() && hero->isAlive()) {
    hero->resetTempDamageMultiplier();
    hero->attack(*enemies[0]);
    if (!enemies[0]->isAlive()) return;
    if (hero->isAlive()) {
      enemies[0]->attack(*hero);
    }
  }
}

void Battle::heroAttackWithMultiplier(int multiplier) {
  if (!isBattleOver() && hero->isAlive()) {
    hero->setTempDamageMultiplier(multiplier);
    hero->attack(*enemies[0]);
    hero->resetTempDamageMultiplier();
    if (!enemies[0]->isAlive()) return;  // монстр мЄртв Ц не атакуем в ответ
    if (hero->isAlive()) {
      enemies[0]->attack(*hero);
    }
  }
}

bool Battle::isHeroAlive() const { return hero->isAlive(); }
Monster* Battle::getMonsterPtr() const {
  return enemies.empty() ? nullptr : enemies[0];
}

void Battle::monstersTurn() {
  for (auto* m : enemies) {
    if (m->isAlive()) {
      m->attack(*hero);
      if (!hero->isAlive()) break;
    }
  }
}
// ѕустые методы дл€ совместимости
void Battle::displayAliveEnemies() {}
void Battle::heroTurn() {}
void Battle::startBattle() {}
