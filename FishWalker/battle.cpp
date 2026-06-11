#include "battle.h"

#include <conio.h>

#include <iostream>

#include "ConsoleUI.h"
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
              << " (HP: " << alive[i]->getHp() << "/" << alive[i]->getMaxHp();
    std::string status = alive[i]->getStatusString();
    if (!status.empty()) std::cout << " " << status;
    std::cout << ")\n";
  }
  std::cout << "> ";
  int choice;
  std::cin >> choice;
  if (choice < 1 || choice > (int)alive.size()) return alive[0];
  return alive[choice - 1];
}

void Battle::basicAttack() {
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
  std::cout << "\n   Attack dealt " << damageDealt << " damage to "
            << target->getName() << "!\n";
  if (!target->isAlive()) {
    std::cout << target->getName() << " is defeated!\n";
  }
}

void Battle::netAttack() {
  if (isBattleOver()) return;

  Weapon* net = hero->getWeaponInSlot(1);
  if (!net) {
    std::cout << "\nNo net equipped!\n";
    return;
  }

  int armorPieces = hero->getArmorPieces();
  int crosses = 1 + armorPieces;
  int speedBonus = armorPieces * SPEED_BONUS_BASE;
  int collected = AimMiniGame::Run(crosses, speedBonus);
  int damagePercent = DAMAGE_PERCENT_BASE + collected * DAMAGE_PERCENT_BONUS;
  hero->setDamagePercent(damagePercent);

  int specialType = net->getSpecialType();
  int specialVal = net->getSpecialValue();

  for (auto* m : enemies) {
    if (!m->isAlive()) continue;

    int oldHp = m->getHp();
    hero->attack(*m);
    int newHp = m->getHp();
    int damageDealt = oldHp - newHp;
    std::cout << "   Net hit " << m->getName() << " for " << damageDealt
              << " damage!\n";

    if (!m->isAlive()) {
      std::cout << m->getName() << " is defeated!\n";
      continue;
    }

    if (specialType == 4) {
      if ((rand() % 100) < specialVal) {
        m->setStunned(true);
        std::cout << m->getName() << " is stunned!\n";
      }
    } else if (specialType == 5) {
      if ((rand() % 100) < specialVal) {
        m->setAtkDebuff(3, 50);
        std::cout << m->getName() << "'s attack is reduced!\n";
      }
    } else if (specialType == 6) {
      if ((rand() % 100) < specialVal) {
        m->setStunned(true);
        m->setAtkDebuff(3, 100);
        std::cout << m->getName() << " is stunned and its attack drops!\n";
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
    ConsoleUI::Pause();
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
  std::cout << "\nHeavy weapon is now recharging for " << HEAVY_ATTACK_COOLDOWN
            << " turns.\n";
}

void Battle::monstersAttack() {
  for (auto* m : enemies) {
    if (m->isAlive() && hero->isAlive()) {
      if (m->isStunned()) {
        continue;
      }
      m->attack(*hero);
      std::cout << m->getName() << " attacks! " << hero->getName()
                << " HP: " << hero->getHp() << "/" << hero->getMaxHp() << "\n";
      if (!hero->isAlive()) break;
    }
  }
}

bool Battle::isHeroAlive() const { return hero->isAlive(); }
