#include "battleUI.h"

static bool fontLoaded = false;
static sf::Font sharedFont;

BattleUI::BattleUI()
    : currentBattle(nullptr),
      heroHpText(nullptr),
      monsterHpText(nullptr),
      actionMenuText(nullptr),
      resultText(nullptr),
      inventoryText(nullptr),
      battleFinished(false),
      showResult(false),
      showInventory(false),
      selectedItemIndex(0),
      waitingForMinigame(false),
      chestMinigameMode(false),
      chestMinigameFinished(false),
      chestMinigameResult(0) {}

BattleUI::~BattleUI() {
  delete heroHpText;
  delete monsterHpText;
  delete actionMenuText;
  delete resultText;
  delete inventoryText;
}

void BattleUI::init(sf::RenderWindow& window, Battle& battle) {
  currentBattle = &battle;
  battleFinished = false;
  showResult = false;
  showInventory = false;
  waitingForMinigame = false;

  if (!fontLoaded) {
    if (!sharedFont.openFromFile("supermario3dworld.otf")) {
    }
    fontLoaded = true;
  }

  delete heroHpText;
  heroHpText = nullptr;
  delete monsterHpText;
  monsterHpText = nullptr;
  delete actionMenuText;
  actionMenuText = nullptr;
  delete resultText;
  resultText = nullptr;
  delete inventoryText;
  inventoryText = nullptr;

  heroHpText = new sf::Text(sharedFont);
  heroHpText->setCharacterSize(24);
  heroHpText->setFillColor(sf::Color::White);
  heroHpText->setPosition(sf::Vector2f(50.f, 50.f));

  monsterHpText = new sf::Text(sharedFont);
  monsterHpText->setCharacterSize(24);
  monsterHpText->setFillColor(sf::Color::White);
  monsterHpText->setPosition(sf::Vector2f(500.f, 50.f));

  actionMenuText = new sf::Text(sharedFont);
  actionMenuText->setCharacterSize(20);
  actionMenuText->setFillColor(sf::Color::Yellow);
  actionMenuText->setString("1 - Attack  2 - Item");
  actionMenuText->setPosition(sf::Vector2f(300.f, 450.f));

  resultText = new sf::Text(sharedFont);
  resultText->setCharacterSize(30);
  resultText->setFillColor(sf::Color::Red);
  resultText->setPosition(sf::Vector2f(300.f, 250.f));

  inventoryText = new sf::Text(sharedFont);
  inventoryText->setCharacterSize(20);
  inventoryText->setFillColor(sf::Color::White);
  inventoryText->setPosition(sf::Vector2f(300.f, 150.f));

  backgroundOverlay.setSize(
      sf::Vector2f(window.getSize().x, window.getSize().y));
  backgroundOverlay.setFillColor(sf::Color(0, 0, 0, 180));
}

void BattleUI::updateBarsAndText() {
  if (!currentBattle) return;
  Hero& hero = currentBattle->getHero();
  Monster& monster = currentBattle->getMonster();

  heroHpText->setString("Hero HP: " + std::to_string(hero.getHp()) + "/" +
                        std::to_string(hero.getMaxHp()) +
                        "  INF: " + std::to_string(hero.getInf()));
  monsterHpText->setString("Monster HP: " + std::to_string(monster.getHp()) +
                           "/" + std::to_string(monster.getMaxHp()));

  float heroPercent = static_cast<float>(hero.getHp()) / hero.getMaxHp();
  float monsterPercent =
      static_cast<float>(monster.getHp()) / monster.getMaxHp();

  heroHpBar.setSize(sf::Vector2f(200.f * heroPercent, 20.f));
  heroHpBar.setFillColor(sf::Color::Green);
  heroHpBar.setPosition(sf::Vector2f(50.f, 80.f));

  monsterHpBar.setSize(sf::Vector2f(200.f * monsterPercent, 20.f));
  monsterHpBar.setFillColor(sf::Color::Red);
  monsterHpBar.setPosition(sf::Vector2f(500.f, 80.f));
}

void BattleUI::showInventoryMenu() {
  showInventory = true;
  selectedItemIndex = 0;
  std::string invStr = "Inventory (press number to use, ESC to close):\n";
  Inventory& inv = currentBattle->getHero().getInventory();
  const auto& items = inv.getItems();
  int i = 1;
  for (const auto& item : items) {
    invStr += std::to_string(i) + ". " + item.getName() + "\n";
    i++;
  }
  inventoryText->setString(invStr);
}

void BattleUI::useSelectedItem() {
  Inventory& inv = currentBattle->getHero().getInventory();
  if (selectedItemIndex >= 0 &&
      selectedItemIndex < static_cast<int>(inv.getItems().size())) {
    if (inv.useItem(selectedItemIndex, currentBattle->getHero())) {
      updateBarsAndText();
    }
  }
  showInventory = false;
}

void BattleUI::finishMinigame() {
  waitingForMinigame = false;

  if (chestMinigameMode) {
    chestMinigameMode = false;
    chestMinigameFinished = true;
    chestMinigameResult = minigame.getDamageMultiplier();
    return;
  }

  if (minigame.isSuccess()) {
    int multiplier = minigame.getDamageMultiplier();
    currentBattle->heroAttackWithMultiplier(multiplier);
  } else {
    if (currentBattle->isHeroAlive()) {
      currentBattle->getMonster().attack(currentBattle->getHero());
    }
  }

  currentBattle->getHero().applyDot();
  updateBarsAndText();

  if (currentBattle->isBattleOver()) {
    showResult = true;
    battleFinished = true;
  }
}

void BattleUI::handleEvent(const sf::Event& event) {
  if (battleFinished) {
    if (event.is<sf::Event::KeyPressed>()) {
      battleFinished = false;
      showResult = false;
    }
    return;
  }

  if (showInventory) {
    if (event.is<sf::Event::KeyPressed>()) {
      const auto* key = event.getIf<sf::Event::KeyPressed>();

      auto sc = key->scancode;
      auto first = sf::Keyboard::Scancode::Num1;
      auto last = sf::Keyboard::Scancode::Num9;

      if (sc >= first && sc <= last) {
        selectedItemIndex = static_cast<int>(sc) - static_cast<int>(first);
        useSelectedItem();
      } else if (sc == sf::Keyboard::Scancode::Escape) {
        showInventory = false;
      }
    }
    return;
  }

  if (waitingForMinigame) {
    if (event.is<sf::Event::KeyPressed>()) {
      const auto* key = event.getIf<sf::Event::KeyPressed>();
      if (key->scancode == sf::Keyboard::Scancode::Space ||
          key->scancode == sf::Keyboard::Scancode::Enter) {
        minigame.stop();
        finishMinigame();
      }
    }
    return;
  }

  if (event.is<sf::Event::KeyPressed>()) {
    const auto* key = event.getIf<sf::Event::KeyPressed>();
    if (key->scancode == sf::Keyboard::Scancode::Num1) {
      minigame.start(MinigamePresets::Attack);
      waitingForMinigame = true;
    } else if (key->scancode == sf::Keyboard::Scancode::Num2) {
      showInventoryMenu();
    }
  }
}

void BattleUI::update(float deltaTime) {
  if (!currentBattle) return;
  if (waitingForMinigame) {
    minigame.update(deltaTime);
    if (!minigame.isActive()) {
      finishMinigame();
    }
  } else {
    updateBarsAndText();
  }
}

void BattleUI::render(sf::RenderWindow& window) {
  window.draw(backgroundOverlay);
  if (heroHpText) window.draw(*heroHpText);
  if (monsterHpText) window.draw(*monsterHpText);
  window.draw(heroHpBar);
  window.draw(monsterHpBar);
  if (actionMenuText) window.draw(*actionMenuText);
  if (showResult && resultText) window.draw(*resultText);
  if (showInventory && inventoryText) window.draw(*inventoryText);
  if (waitingForMinigame) {
    minigame.render(window);
  }
}

void BattleUI::startChestMinigame() {
  chestMinigameMode = true;
  chestMinigameFinished = false;
  chestMinigameResult = 0;
  minigame.start(MinigamePresets::Chest);
  waitingForMinigame = true;
}

bool BattleUI::isMinigameRunning() const { return waitingForMinigame; }
bool BattleUI::isChestMinigameFinished() const { return chestMinigameFinished; }
int BattleUI::getChestMinigameResult() const { return chestMinigameResult; }
bool BattleUI::isFinished() const { return battleFinished; }
