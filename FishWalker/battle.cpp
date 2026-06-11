#include "battle.h"

#include <conio.h>

#include <iostream>

#include "aimminigame.h"
#include "constants.h"

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

Monster* Battle::selectTarget() const {
  std::vector<Monster*> alive;
  for (auto* m : enemies)
    if (m->isAlive()) alive.push_back(m);
  if (alive.empty()) return nullptr;
  if (alive.size() == 1) return alive[0];
  std::cout << "\nChoose target:\n";
  for (size_t i = 0; i < alive.size(); ++i) {
    std::cout << i + 1 << ". " << alive[i]->getName()
              << " (HP: " << alive[i]->getHp() << "/" << alive[i]->getMaxHp()
              << ")\n";
  }
  std::cout << "> ";
  int choice;
  std::cin >> choice;
  if (choice < 1 || choice > (int)alive.size()) return alive[0];
  return alive[choice - 1];
}

void Battle::rodAttack() {
  if (isBattleOver()) return;
  Monster* target = selectTarget();
  if (!target || !target->isAlive()) return;

  int armorPieces = hero->getArmorPieces();
  int crosses = 1 + armorPieces;
  int speedBonus = armorPieces * SPEED_BONUS_BASE;
  int collected = AimMiniGame::Run(crosses, speedBonus);
  int damagePercent = DAMAGE_PERCENT_BASE + collected * DAMAGE_PERCENT_BONUS;
  hero->setDamagePercent(damagePercent);
  int oldHp = target->getHp();
  hero->attack(*target);
  int newHp = target->getHp();
  int damageDealt = oldHp - newHp;
  std::cout << "\n   Rod attack dealt " << damageDealt << " damage to "
            << target->getName() << "!\n";
  if (!target->isAlive()) {
    std::cout << target->getName() << " is defeated!\n";
  }
}

void Battle::netAttack() {
  if (isBattleOver()) return;
  int armorPieces = hero->getArmorPieces();
  int crosses = 1 + armorPieces;
  int speedBonus = armorPieces * SPEED_BONUS_BASE;
  int collected = AimMiniGame::Run(crosses, speedBonus);
  int damagePercent = DAMAGE_PERCENT_BASE + collected * DAMAGE_PERCENT_BONUS;
  hero->setDamagePercent(damagePercent);
  for (auto* m : enemies) {
    if (m->isAlive()) {
      int oldHp = m->getHp();
      hero->attack(*m);
      int newHp = m->getHp();
      int damageDealt = oldHp - newHp;
      std::cout << "   Net hit " << m->getName() << " for " << damageDealt
                << " damage!\n";
      if (!m->isAlive()) {
        std::cout << m->getName() << " is defeated!\n";
      }
    }
  }
  hero->resetDamagePercent();
}

void Battle::heavyAttack() {
  if (isBattleOver()) return;
  if (hero->getHeavyCooldown() > 0) {
    std::cout << "\nHeavy weapon recharging! " << hero->getHeavyCooldown()
              << " turns left.\n";
    return;
  }
  Monster* target = selectTarget();
  if (!target || !target->isAlive()) return;

  int armorPieces = hero->getArmorPieces();
  int crosses = 1 + armorPieces;
  int speedBonus = armorPieces * SPEED_BONUS_BASE;
  int collected = AimMiniGame::Run(crosses, speedBonus);
  int damagePercent = DAMAGE_PERCENT_BASE + collected * DAMAGE_PERCENT_BONUS;
  hero->setDamagePercent(damagePercent);
  int oldHp = target->getHp();
  hero->attack(*target);
  int newHp = target->getHp();
  int damageDealt = oldHp - newHp;
  std::cout << "\n   Heavy attack dealt " << damageDealt << " damage to "
            << target->getName() << "!\n";
  if (!target->isAlive()) {
    std::cout << target->getName() << " is defeated!\n";
  }
  hero->setHeavyCooldown(HEAVY_ATTACK_COOLDOWN);
}

void Battle::monstersAttack() {
  for (auto* m : enemies) {
    if (m->isAlive() && hero->isAlive()) {
      m->attack(*hero);
      std::cout << m->getName() << " attacks! " << hero->getName()
                << " HP: " << hero->getHp() << "/" << hero->getMaxHp() << "\n";
      if (!hero->isAlive()) break;
    }
  }
}

bool Battle::isHeroAlive() const { return hero->isAlive(); }
