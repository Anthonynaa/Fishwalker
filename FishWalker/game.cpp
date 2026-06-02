#include "game.h"

#include <cstdlib>
#include <ctime>
#include <string>

int getNumberKeyIndex(sf::Keyboard::Scancode scancode) {
  using Sc = sf::Keyboard::Scancode;

  if (scancode >= Sc::Num1 && scancode <= Sc::Num9)
    return static_cast<int>(scancode) - static_cast<int>(Sc::Num1);

  return -1;
}

Game::Game()
    : window(sf::VideoMode({MAP_WIDTH * CELL_SIZE, MAP_HEIGHT * CELL_SIZE}),
             "Fishwalker"),
      heroX(MAP_WIDTH / 2),
      heroY(MAP_HEIGHT / 2),
      hero("Fisherman", HERO_ID, HERO_START_HP, HERO_MAX_HP, HERO_ATK, HERO_INF,
           nullptr, nullptr),
      inBattle(false),
      showMapInventory(false),
      knifeFound(false),
      armorFound(false),
      gameWon(false),
      showVictory(false),
      gameOver(false),
      showGameOver(false),
      openingChestIndex(-1),
      chestMessageTimer(0.f),
      currentBattle(nullptr),
      mapInventoryText(nullptr) {
  window.setFramerateLimit(60);
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  (void)wallTexture.loadFromFile("assets/wall.png");
  (void)floorTexture.loadFromFile("assets/floor.png");
  (void)chestTexture.loadFromFile("assets/chest.png");
  (void)monsterNormalTexture.loadFromFile("assets/normal_monster.png");
  (void)monsterInfectedTexture.loadFromFile("assets/infected_monster.png");

  for (int y = 0; y < MAP_HEIGHT; ++y)
    for (int x = 0; x < MAP_WIDTH; ++x) map[y][x] = 0;
  for (int x = 0; x < MAP_WIDTH; ++x) {
    map[0][x] = 1;
    map[MAP_HEIGHT - 1][x] = 1;
  }
  for (int y = 0; y < MAP_HEIGHT; ++y) {
    map[y][0] = 1;
    map[y][MAP_WIDTH - 1] = 1;
  }
  for (int i = 0; i < RANDOM_WALL_COUNT; ++i) {
    int x = rand() % (MAP_WIDTH - 2) + 1;
    int y = rand() % (MAP_HEIGHT - 2) + 1;
    map[y][x] = 1;
  }

  std::vector<std::pair<int, int>> freeCells;
  for (int y = 1; y < MAP_HEIGHT - 1; ++y)
    for (int x = 1; x < MAP_WIDTH - 1; ++x)
      if (map[y][x] == 0 && !(x == heroX && y == heroY))
        freeCells.push_back({x, y});
  for (int i = 0; i < MONSTER_COUNT && i < (int)freeCells.size(); ++i) {
    int idx = rand() % freeCells.size();
    int mx = freeCells[idx].first, my = freeCells[idx].second;
    Monster* m = (rand() % 2 == 0) ? monsterFactory.createNormalMonster()
                                   : monsterFactory.createInfectedMonster();
    monsters.push_back({m, mx, my, true, 0.f});
    freeCells.erase(freeCells.begin() + idx);
  }
  for (int i = 0; i < CHEST_COUNT && i < (int)freeCells.size(); ++i) {
    int idx = rand() % freeCells.size();
    int bx = freeCells[idx].first, by = freeCells[idx].second;
    boxes.push_back({bx, by, true, createUniqueItem()});
    freeCells.erase(freeCells.begin() + idx);
  }
  if (map[heroY][heroX] == 1) {
    for (int y = 1; y < MAP_HEIGHT - 1; ++y)
      for (int x = 1; x < MAP_WIDTH - 1; ++x)
        if (map[y][x] == 0) {
          heroX = x;
          heroY = y;
          break;
        }
  }

  camera.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
  camera.setCenter(sf::Vector2f(heroX * CELL_SIZE, heroY * CELL_SIZE));

  static sf::Font font;
  if (!font.openFromFile("supermario3dworld.otf")) {
  }
  mapInventoryText = new sf::Text(font);
  mapInventoryText->setCharacterSize(20);
  mapInventoryText->setFillColor(sf::Color::White);
}

Game::~Game() {
  for (auto& m : monsters) delete m.ptr;
  delete mapInventoryText;
  delete currentBattle;
}

Item Game::createUniqueItem() {
  Item loot = itemFactory.createItem();
  if (loot.getType() == ItemType::WEAPON) {
    if (knifeFound)
      loot = itemFactory.createHealthPotion();
    else
      knifeFound = true;
  } else if (loot.getType() == ItemType::ARMOR) {
    if (armorFound)
      loot = itemFactory.createHealthPotion();
    else
      armorFound = true;
  }
  return loot;
}

void Game::showGameOverScreen() {
  showGameOver = true;
  gameOver = true;
}
void Game::showVictoryScreen() {
  showVictory = true;
  gameWon = true;
}

void Game::run() {
  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();
    processInput();
    if (!gameOver) update(dt);
    render();
  }
}

void Game::openInventoryOnMap() {
  showMapInventory = true;
  std::string invStr = "Inventory (press number to use, ESC to close):\n";
  const auto& items = hero.getInventory().getItems();
  int i = 1;
  for (const auto& item : items) {
    invStr += std::to_string(i) + ". " + item.getName();
    invStr += item.getDescription();
    invStr += "\n";
    ++i;
  }
  mapInventoryText->setString(invStr);
  mapInventoryText->setPosition(sf::Vector2f(300.f, 150.f));
}

void Game::processInput() {
  while (const auto event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
      continue;
    }
    if (gameWon && showVictory) {
      if (event->is<sf::Event::KeyPressed>()) window.close();
      continue;
    }
    if (gameOver && showGameOver) {
      if (event->is<sf::Event::KeyPressed>()) window.close();
      continue;
    }
    if (inBattle || battleUI.isMinigameRunning()) {
      battleUI.handleEvent(*event);
      continue;
    }
    if (showMapInventory) {
      if (event->is<sf::Event::KeyPressed>()) {
        const auto* key = event->getIf<sf::Event::KeyPressed>();

        int num = getNumberKeyIndex(key->scancode);

        if (num != -1 && num < (int)hero.getInventory().getItems().size()) {
          if (hero.getInventory().useItem(num, hero)) openInventoryOnMap();
        } else if (key->scancode == sf::Keyboard::Scancode::Escape) {
          showMapInventory = false;
        }
      }
      continue;
    }
    if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
      if (key->scancode == sf::Keyboard::Scancode::I) {
        openInventoryOnMap();
        continue;
      }
      int newX = heroX, newY = heroY;
      if (key->scancode == sf::Keyboard::Scancode::Up) newY--;
      if (key->scancode == sf::Keyboard::Scancode::Down) newY++;
      if (key->scancode == sf::Keyboard::Scancode::Left) newX--;
      if (key->scancode == sf::Keyboard::Scancode::Right) newX++;
      if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT &&
          map[newY][newX] != 1) {
        for (int i = 0; i < (int)boxes.size(); ++i) {
          if (boxes[i].active && newX == boxes[i].x && newY == boxes[i].y) {
            battleUI.startChestMinigame();
            openingChestIndex = i;
            return;
          }
        }
        bool monsterHere = false;
        for (auto& m : monsters) {
          if (m.alive && newX == m.x && newY == m.y) {
            delete currentBattle;
            currentBattle = new Battle(&hero, std::vector<Monster*>{m.ptr});
            battleUI.init(window, *currentBattle);
            inBattle = true;
            monsterHere = true;
            break;
          }
        }
        if (!monsterHere) {
          heroX = newX;
          heroY = newY;
          hero.applyDot();
          if (!hero.isAlive()) showGameOverScreen();
        }
      }
    }
  }
}

void Game::moveMonsters(float deltaTime) {
  const float MOVE_INTERVAL = 2.f;
  for (auto& m : monsters) {
    if (!m.alive) continue;
    m.moveTimer += deltaTime;
    if (m.moveTimer >= MOVE_INTERVAL) {
      m.moveTimer = 0.f;
      int dx = 0, dy = 0;
      int dir = rand() % 4;
      if (dir == 0)
        dy = -1;
      else if (dir == 1)
        dy = 1;
      else if (dir == 2)
        dx = -1;
      else
        dx = 1;
      int newX = m.x + dx, newY = m.y + dy;
      if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT &&
          map[newY][newX] != 1) {
        bool occupied = (newX == heroX && newY == heroY);
        for (const auto& other : monsters) {
          if (other.alive && other.x == newX && other.y == newY)
            occupied = true;
        }
        if (!occupied) {
          m.x = newX;
          m.y = newY;
        }
      }
    }
  }
}

void Game::update(float deltaTime) {
  if (battleUI.isMinigameRunning() || inBattle) {
    battleUI.update(deltaTime);
  }
  if (battleUI.isChestMinigameFinished()) {
    int result = battleUI.getChestMinigameResult();
    if (openingChestIndex != -1) {
      auto& chest = boxes[openingChestIndex];
      if (result == 2) {
        hero.getInventory().addItem(chest.loot);
        hero.getInventory().addItem(chest.loot);
        chestMessage = "Perfect! You got 2 items!";
      } else if (result == 1) {
        hero.getInventory().addItem(chest.loot);
        chestMessage = "Good! You got 1 item!";
      } else {
        chestMessage = "Miss! No item received";
      }
      chestMessageTimer = CHEST_MESSAGE_TIMER;
      chest.active = false;
      openingChestIndex = -1;
    }
  }
  if (inBattle) {
    if (battleUI.isFinished()) {
      inBattle = false;
      if (!hero.isAlive()) {
        showGameOverScreen();
      } else {
        Monster* deadMonster = currentBattle->getMonsterPtr();
        for (auto& m : monsters) {
          if (m.ptr == deadMonster) {
            m.alive = false;
            delete m.ptr;
            m.ptr = nullptr;
            break;
          }
        }
        bool allDead = true;
        for (const auto& m : monsters)
          if (m.alive) {
            allDead = false;
            break;
          }
        if (allDead) showVictoryScreen();
      }
      delete currentBattle;
      currentBattle = nullptr;
    }
    return;
  }
  if (battleUI.isMinigameRunning()) {
    if (battleUI.isChestMinigameFinished()) return;
  }
  moveMonsters(deltaTime);
  if (chestMessageTimer > 0.f) {
    chestMessageTimer -= deltaTime;
    if (chestMessageTimer <= 0.f) chestMessage.clear();
  }
  if (!gameOver) {
    float targetX = heroX * CELL_SIZE, targetY = heroY * CELL_SIZE;
    sf::Vector2f center = camera.getCenter();
    center.x += (targetX - center.x) * CAMERA_SPEED;
    center.y += (targetY - center.y) * CAMERA_SPEED;
    camera.setCenter(center);
    window.setView(camera);
  }
}

void Game::render() {
  window.clear();

  if (gameWon && showVictory) {
    static sf::Font victoryFont;
    static bool fontLoaded = false;
    if (!fontLoaded)
      fontLoaded = victoryFont.openFromFile("supermario3dworld.otf");
    if (fontLoaded) {
      sf::Text text(victoryFont);
      text.setCharacterSize(50);
      text.setFillColor(sf::Color::Green);
      text.setString("VICTORY!\nAll monsters defeated\nPress any key to exit");
      sf::FloatRect bounds = text.getLocalBounds();
      text.setPosition({(window.getSize().x - bounds.size.x) / 2,
                        (window.getSize().y - bounds.size.y) / 2});
      window.setView(window.getDefaultView());
      window.draw(text);
    }
    window.display();
    return;
  }

  if (gameOver && showGameOver) {
    static sf::Font gameOverFont;
    static bool fontLoaded = false;
    if (!fontLoaded)
      fontLoaded = gameOverFont.openFromFile("supermario3dworld.otf");
    if (fontLoaded) {
      sf::Text text(gameOverFont);
      text.setCharacterSize(50);
      text.setFillColor(sf::Color::Red);
      text.setString("GAME OVER\nPress any key to exit");
      sf::FloatRect bounds = text.getLocalBounds();
      text.setPosition({(window.getSize().x - bounds.size.x) / 2,
                        (window.getSize().y - bounds.size.y) / 2});
      window.draw(text);
    }
    window.display();
    return;
  }

  sf::Sprite floorSprite(floorTexture);
  sf::Sprite wallSprite(wallTexture);
  sf::Sprite chestSprite(chestTexture);
  sf::Sprite monsterNormalSprite(monsterNormalTexture);
  sf::Sprite monsterInfectedSprite(monsterInfectedTexture);

  auto scaleToCell = [&](sf::Sprite& s) {
    s.setScale({(float)CELL_SIZE / s.getTexture().getSize().x,
                (float)CELL_SIZE / s.getTexture().getSize().y});
  };
  scaleToCell(floorSprite);
  scaleToCell(wallSprite);
  scaleToCell(chestSprite);
  scaleToCell(monsterNormalSprite);
  scaleToCell(monsterInfectedSprite);

  for (int y = 0; y < MAP_HEIGHT; ++y) {
    for (int x = 0; x < MAP_WIDTH; ++x) {
      sf::Sprite& tileSprite = (map[y][x] == 1) ? wallSprite : floorSprite;
      tileSprite.setPosition({(float)(x * CELL_SIZE), (float)(y * CELL_SIZE)});
      window.draw(tileSprite);
    }
  }

  for (const auto& box : boxes) {
    if (!box.active) continue;
    chestSprite.setPosition(
        {(float)(box.x * CELL_SIZE), (float)(box.y * CELL_SIZE)});
    window.draw(chestSprite);
  }

  for (const auto& m : monsters) {
    if (m.alive) {
      sf::Sprite monsterSprite(dynamic_cast<InfectedMonster*>(m.ptr)
                                   ? monsterInfectedTexture
                                   : monsterNormalTexture);
      float scaleX = (float)CELL_SIZE / monsterSprite.getTexture().getSize().x;
      float scaleY = (float)CELL_SIZE / monsterSprite.getTexture().getSize().y;
      monsterSprite.setScale({scaleX, scaleY});
      monsterSprite.setPosition(
          {(float)(m.x * CELL_SIZE), (float)(m.y * CELL_SIZE)});
      window.draw(monsterSprite);
    }
  }

  sf::RectangleShape heroRect({(float)CELL_SIZE, (float)CELL_SIZE});
  heroRect.setFillColor(sf::Color::White);
  heroRect.setPosition(
      {(float)(heroX * CELL_SIZE), (float)(heroY * CELL_SIZE)});
  window.draw(heroRect);

  static sf::Font hudFont;
  static bool hudFontLoaded = false;
  if (!hudFontLoaded)
    hudFontLoaded = hudFont.openFromFile("supermario3dworld.otf");
  if (hudFontLoaded) {
    sf::Text hudText(hudFont);
    hudText.setCharacterSize(20);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition({10.f, 10.f});
    hudText.setString("HP: " + std::to_string(hero.getHp()) + "/" +
                      std::to_string(hero.getMaxHp()) +
                      "  INF: " + std::to_string(hero.getInf()) + "/100");
    window.setView(window.getDefaultView());
    window.draw(hudText);
    window.setView(camera);
  }

  if (showMapInventory) {
    window.setView(window.getDefaultView());
    sf::RectangleShape overlay(
        {(float)window.getSize().x, (float)window.getSize().y});
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);
    if (mapInventoryText) window.draw(*mapInventoryText);
    window.setView(camera);
  }

  if (inBattle) {
    window.setView(window.getDefaultView());
    battleUI.render(window);
    window.setView(camera);
  }

  if (battleUI.isMinigameRunning()) {
    window.setView(window.getDefaultView());
    battleUI.render(window);
    window.setView(camera);
  }

  if (!chestMessage.empty()) {
    static sf::Font font;
    static bool loaded = false;
    if (!loaded) loaded = font.openFromFile("supermario3dworld.otf");
    if (loaded) {
      sf::Text text(font);
      text.setString(chestMessage);
      text.setCharacterSize(24);
      text.setFillColor(sf::Color::Yellow);
      text.setPosition({20.f, 60.f});
      window.setView(window.getDefaultView());
      window.draw(text);
      window.setView(camera);
    }
  }

  window.display();
}
