#include "game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

Game::Game()
    : window(sf::VideoMode({MAP_WIDTH * CELL_SIZE, MAP_HEIGHT * CELL_SIZE}),
             "Fishwalker"),
      heroX(MAP_WIDTH / 2),
      heroY(MAP_HEIGHT / 2),
      hero("Fisherman", 1, HERO_START_HP, HERO_MAX_HP, HERO_ATK, HERO_ACC,
           HERO_AGL, HERO_RES, HERO_INF, HERO_CRIT, nullptr, nullptr),
      inBattle(false),
      lastFrameTime(0.f),
      showMapInventory(false),
      selectedMapItemIndex(0),
      knifeFound(false),
      armorFound(false),
      gameOver(false),
      showGameOver(false) {
  window.setFramerateLimit(60);
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  if (!wallTexture.loadFromFile("assets/wall.png"))
    std::cout << "Failed to load wall\n";

  if (!floorTexture.loadFromFile("assets/floor.png"))
    std::cout << "Failed to load floor\n";

  if (!chestTexture.loadFromFile("assets/chest.png"))
    std::cout << "Failed to load chest\n";

  if (!monsterNormalTexture.loadFromFile("assets/normal_monster.png"))
    std::cout << "Failed to load monster normal\n";

  if (!monsterInfectedTexture.loadFromFile("assets/infected_monster.png"))
    std::cout << "Failed to load monster infected\n";

  // 1. Заполняем карту полом (0)
  for (int y = 0; y < MAP_HEIGHT; ++y)
    for (int x = 0; x < MAP_WIDTH; ++x) map[y][x] = 0;

  // 2. Стены по краям
  for (int x = 0; x < MAP_WIDTH; ++x) {
    map[0][x] = 1;
    map[MAP_HEIGHT - 1][x] = 1;
  }
  for (int y = 0; y < MAP_HEIGHT; ++y) {
    map[y][0] = 1;
    map[y][MAP_WIDTH - 1] = 1;
  }

  // 3. Внутренние стены (лабиринт) – не перекрывать возможные сундуки, сначала
  // стены
  for (int i = 0; i < 30; ++i) {
    int x = rand() % (MAP_WIDTH - 2) + 1;
    int y = rand() % (MAP_HEIGHT - 2) + 1;
    map[y][x] = 1;
  }

  // 4. Размещаем монстров (только на свободных клетках, не стенах и не на
  // сундуках)
  std::vector<std::pair<int, int>> freeCells;
  for (int y = 1; y < MAP_HEIGHT - 1; ++y)
    for (int x = 1; x < MAP_WIDTH - 1; ++x)
      if (map[y][x] == 0 && !(x == heroX && y == heroY))
        freeCells.push_back({x, y});

  // Монстры
  for (int i = 0; i < 4 && i < (int)freeCells.size(); ++i) {
    int idx = rand() % freeCells.size();
    int mx = freeCells[idx].first, my = freeCells[idx].second;
    Monster* m = (rand() % 2 == 0) ? monsterFactory.createNormalMonster()
                                   : monsterFactory.createInfectedMonster();
    monsters.push_back({m, mx, my, true, 0.f});
    // Удаляем эту клетку из списка свободных, чтобы не ставить сундук туда же
    freeCells.erase(freeCells.begin() + idx);
  }

  // 5. Размещаем сундуки (на оставшихся свободных клетках)
  for (int i = 0; i < 3 && i < (int)freeCells.size(); ++i) {
    int idx = rand() % freeCells.size();
    int bx = freeCells[idx].first, by = freeCells[idx].second;
    boxes.push_back({bx, by, true, createUniqueItem()});
    freeCells.erase(freeCells.begin() + idx);
  }

  // 6. Герой начинается с центра (но если там стена – ищем первую свободную)
  if (map[heroY][heroX] == 1) {
    for (int y = 1; y < MAP_HEIGHT - 1; ++y)
      for (int x = 1; x < MAP_WIDTH - 1; ++x)
        if (map[y][x] == 0) {
          heroX = x;
          heroY = y;
          break;
        }
  }

  // Камера
  camera.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
  camera.setCenter(sf::Vector2f(heroX * CELL_SIZE, heroY * CELL_SIZE));

  static sf::Font font;
  if (!font.openFromFile("arial.ttf")) {
  }
  mapInventoryText = std::make_unique<sf::Text>(font);
  mapInventoryText->setCharacterSize(20);
  mapInventoryText->setFillColor(sf::Color::White);
}

Game::~Game() {
  for (auto& m : monsters) delete m.ptr;
}

Item Game::createUniqueItem() {
  Item loot = itemFactory.createItem();
  if (loot.getType() == ItemType::WEAPON) {
    if (knifeFound) {
      loot = itemFactory.createHealthPotion();
    } else {
      knifeFound = true;
    }
  } else if (loot.getType() == ItemType::ARMOR) {
    if (armorFound) {
      loot = itemFactory.createHealthPotion();
    } else {
      armorFound = true;
    }
  }
  return loot;
}

void Game::showGameOverScreen() {
  showGameOver = true;
  gameOver = true;
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
  selectedMapItemIndex = 0;
  std::string invStr = "Inventory (press number to use, ESC to close):\n";
  const auto& items = hero.getInventory().getItems();
  int i = 1;
  for (const auto& item : items) {
    invStr += std::to_string(i) + ". " + item.getName();
    if (item.getType() == ItemType::HEAL)
      invStr += " (heal +" + std::to_string(item.getValue()) + " HP)";
    else if (item.getType() == ItemType::ANTIDOTE)
      invStr += " (antidote -" + std::to_string(item.getValue()) + " INF)";
    else if (item.getType() == ItemType::WEAPON)
      invStr += " (+" + std::to_string(item.getValue()) + " ATK)";
    else if (item.getType() == ItemType::ARMOR)
      invStr += " (+" + std::to_string(item.getValue()) + " DEF)";
    invStr += "\n";
    i++;
  }
  mapInventoryText->setString(invStr);
  mapInventoryText->setPosition(sf::Vector2f(300.f, 150.f));
}

void Game::processInput() {
  while (const auto event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) window.close();

    if (gameOver && showGameOver) {
      if (event->is<sf::Event::KeyPressed>()) {
        window.close();
      }
      continue;
    }

    if (inBattle) {
      battleUI.handleEvent(*event);
    } else if (showMapInventory) {
      if (event->is<sf::Event::KeyPressed>()) {
        const auto* key = event->getIf<sf::Event::KeyPressed>();
        int num = -1;
        if (key->scancode == sf::Keyboard::Scancode::Num1)
          num = 0;
        else if (key->scancode == sf::Keyboard::Scancode::Num2)
          num = 1;
        else if (key->scancode == sf::Keyboard::Scancode::Num3)
          num = 2;
        else if (key->scancode == sf::Keyboard::Scancode::Num4)
          num = 3;
        else if (key->scancode == sf::Keyboard::Scancode::Num5)
          num = 4;
        else if (key->scancode == sf::Keyboard::Scancode::Num6)
          num = 5;
        else if (key->scancode == sf::Keyboard::Scancode::Num7)
          num = 6;
        else if (key->scancode == sf::Keyboard::Scancode::Num8)
          num = 7;
        else if (key->scancode == sf::Keyboard::Scancode::Num9)
          num = 8;
        if (num != -1 && num < (int)hero.getInventory().getItems().size()) {
          if (hero.getInventory().useItem(num, hero)) {
            openInventoryOnMap();
          }
        } else if (key->scancode == sf::Keyboard::Scancode::Escape) {
          showMapInventory = false;
        }
      }
    } else {
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
          // Проверяем сундуки
          for (auto& box : boxes) {
            if (box.active && newX == box.x && newY == box.y) {
              hero.getInventory().addItem(box.loot);
              box.active = false;
              break;
            }
          }
          bool monsterHere = false;
          for (auto& m : monsters) {
            if (m.alive && newX == m.x && newY == m.y) {
              currentBattle =
                  std::make_unique<Battle>(&hero, std::vector<Monster*>{m.ptr});
              battleUI.init(window, *currentBattle);
              inBattle = true;
              monsterHere = true;
              break;
            }
          }
          if (!monsterHere) {
            heroX = newX;
            heroY = newY;
            // DoT при движении
            hero.applyDot();
            if (!hero.isAlive()) {
              showGameOverScreen();
            }
          }
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
      int newX = m.x + dx;
      int newY = m.y + dy;
      if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT &&
          map[newY][newX] != 1) {
        bool occupied = false;
        if (newX == heroX && newY == heroY) occupied = true;
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
  if (inBattle) {
    battleUI.update(deltaTime);
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
      }
      currentBattle.reset();
    }
  } else {
    moveMonsters(deltaTime);
  }

  if (!gameOver) {
    float targetX = heroX * CELL_SIZE;
    float targetY = heroY * CELL_SIZE;
    sf::Vector2f center = camera.getCenter();
    center.x += (targetX - center.x) * CAMERA_SPEED;
    center.y += (targetY - center.y) * CAMERA_SPEED;
    camera.setCenter(center);
    window.setView(camera);
  }
}

void Game::render() {
  window.clear();

  // GAME OVER SCREEN

  if (gameOver && showGameOver) {
    static sf::Font gameOverFont;
    static bool fontLoaded = false;
    if (!fontLoaded) fontLoaded = gameOverFont.openFromFile("arial.ttf");

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

  // масштаб под клетку
  auto scaleToCell = [&](sf::Sprite& s) {
    s.setScale({(float)CELL_SIZE / s.getTexture().getSize().x,
                (float)CELL_SIZE / s.getTexture().getSize().y});
  };

  scaleToCell(floorSprite);
  scaleToCell(wallSprite);
  scaleToCell(chestSprite);
  scaleToCell(monsterNormalSprite);
  scaleToCell(monsterInfectedSprite);

  // КАРТА (ПОЛ + СТЕНЫ)

  for (int y = 0; y < MAP_HEIGHT; ++y) {
    for (int x = 0; x < MAP_WIDTH; ++x) {
      sf::Sprite& tileSprite = (map[y][x] == 1) ? wallSprite : floorSprite;

      tileSprite.setPosition({(float)(x * CELL_SIZE), (float)(y * CELL_SIZE)});

      window.draw(tileSprite);
    }
  }

  // СУНДУКИ

  for (const auto& box : boxes) {
    if (!box.active) continue;

    chestSprite.setPosition(
        {(float)(box.x * CELL_SIZE), (float)(box.y * CELL_SIZE)});

    window.draw(chestSprite);
  }

  // МОНСТРЫ (как раньше, но с текстурами)

  for (const auto& m : monsters) {
    if (m.alive) {
      sf::Sprite monsterSprite(dynamic_cast<InfectedMonster*>(m.ptr)
                                   ? monsterInfectedTexture
                                   : monsterNormalTexture);

      float scaleX = static_cast<float>(CELL_SIZE) /
                     monsterSprite.getTexture().getSize().x;

      float scaleY = static_cast<float>(CELL_SIZE) /
                     monsterSprite.getTexture().getSize().y;

      monsterSprite.setScale({scaleX, scaleY});

      monsterSprite.setPosition({static_cast<float>(m.x * CELL_SIZE),
                                 static_cast<float>(m.y * CELL_SIZE)});

      window.draw(monsterSprite);
    }
  }

  // ГЕРОЙ (ПОКА БЕЗ ТЕКСТУРЫ)

  sf::RectangleShape heroRect({(float)CELL_SIZE, (float)CELL_SIZE});
  heroRect.setFillColor(sf::Color::Green);
  heroRect.setPosition({static_cast<float>(heroX * CELL_SIZE),
                        static_cast<float>(heroY * CELL_SIZE)});
  window.draw(heroRect);

  // HUD

  static sf::Font hudFont;
  static bool hudFontLoaded = false;
  if (!hudFontLoaded) hudFontLoaded = hudFont.openFromFile("arial.ttf");

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

  // INVENTORY OVERLAY

  if (showMapInventory) {
    window.setView(window.getDefaultView());

    sf::RectangleShape overlay(
        {(float)window.getSize().x, (float)window.getSize().y});
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    if (mapInventoryText) window.draw(*mapInventoryText);

    window.setView(camera);
  }

  // BATTLE UI

  if (inBattle) {
    window.setView(window.getDefaultView());
    battleUI.render(window);
    window.setView(camera);
  }

  window.display();
}
