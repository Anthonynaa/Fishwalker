#include "aimminigame.h"

#include <conio.h>
#include <windows.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "constants.h"

static void gotoxy(int x, int y) {
  COORD coord;
  coord.X = (SHORT)x;
  coord.Y = (SHORT)y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

static void ShowConsoleCursor(bool show) {
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
  cursorInfo.bVisible = show;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

static void drawCellContent(int row, bool isTarget) {
  if (!isTarget) {
    for (int i = 0; i < CELL_SIZE; ++i) std::cout << NORMAL_CHAR;
    return;
  }
  static const char targetPattern[CELL_SIZE][CELL_SIZE] = {
      {' ', '+', ' '}, {'+', '+', '+'}, {' ', '+', ' '}};
  for (int i = 0; i < CELL_SIZE; ++i) {
    char c = targetPattern[row][i];
    if (c == '+')
      std::cout << TARGET_CROSS;
    else
      std::cout << TARGET_BG;
  }
}

static void drawGrid(int currentPos, const std::vector<bool>& isTargetCell,
                     int cellsCount, int screenWidth) {
  int totalWidth = cellsCount * (CELL_SIZE + 2) + (cellsCount - 1);
  int leftMargin = (screenWidth - totalWidth) / 2;
  if (leftMargin < 0) leftMargin = 0;

  gotoxy(0, 4);
  std::cout << std::string(leftMargin, ' ')
            << "Press any key to collect the cross! Crosses left: "
            << std::count(isTargetCell.begin(), isTargetCell.end(), true)
            << "\n";

  for (int row = 0; row < CELL_SIZE + 2; ++row) {
    gotoxy(0, 6 + row);
    std::cout << std::string(leftMargin, ' ');

    for (int col = 0; col < cellsCount; ++col) {
      bool isCurrent = (col == currentPos);

      if (isCurrent) {
        if (row == 0) {
          std::cout << FRAME_CORNER << std::string(CELL_SIZE, FRAME_TOP)
                    << FRAME_CORNER;
        } else if (row == CELL_SIZE + 1) {
          std::cout << FRAME_CORNER << std::string(CELL_SIZE, FRAME_BOTTOM)
                    << FRAME_CORNER;
        } else {
          std::cout << FRAME_LEFT;
          drawCellContent(row - 1, isTargetCell[col]);
          std::cout << FRAME_RIGHT;
        }
      } else {
        if (row == 0 || row == CELL_SIZE + 1) {
          std::cout << std::string(CELL_SIZE + 2, ' ');
        } else {
          std::cout << ' ';
          drawCellContent(row - 1, isTargetCell[col]);
          std::cout << ' ';
        }
      }
      if (col < cellsCount - 1) std::cout << ' ';
    }
    std::cout << "\n";
  }
  std::cout.flush();
}

int AimMiniGame::Run(int crossesCount, int speedBonusPercent) {
  static bool seeded = false;
  if (!seeded) {
    srand((unsigned)time(nullptr));
    seeded = true;
  }

  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  int screenWidth = csbi.dwSize.X;

  int maxCells = (screenWidth + 1) / (CELL_SIZE + 3);
  int cellsCount = FIXED_CELLS_COUNT;
  if (cellsCount > maxCells) cellsCount = maxCells;
  if (cellsCount < 1) cellsCount = 1;

  crossesCount = (crossesCount < 1)
                     ? 1
                     : (crossesCount > cellsCount ? cellsCount : crossesCount);

  std::vector<bool> isTargetCell(cellsCount, false);
  for (int i = 0; i < crossesCount; ++i) {
    int idx;
    do {
      idx = rand() % cellsCount;
    } while (isTargetCell[idx]);
    isTargetCell[idx] = true;
  }

  int currentPos = 0;
  int stepCount = 0;
  int collected = 0;
  int maxSteps = cellsCount * MAX_CYCLES;

  int moveInterval = BASE_MOVE_INTERVAL_MS * 100 / (100 + speedBonusPercent);
  if (moveInterval < 20) moveInterval = 20;

  system("cls");
  ShowConsoleCursor(false);
  drawGrid(currentPos, isTargetCell, cellsCount, screenWidth);

  int nextMoveTime = GetTickCount64() + moveInterval;

  while (true) {
    int now = GetTickCount64();
    if (now >= nextMoveTime) {
      currentPos = (currentPos + 1) % cellsCount;
      stepCount++;
      drawGrid(currentPos, isTargetCell, cellsCount, screenWidth);
      nextMoveTime = now + moveInterval;
    }

    if (_kbhit()) {
      _getch();
      if (isTargetCell[currentPos]) {
        isTargetCell[currentPos] = false;
        collected++;
        drawGrid(currentPos, isTargetCell, cellsCount, screenWidth);
        if (collected >= crossesCount) break;
      } else {
        break;
      }
    }

    if (stepCount >= maxSteps) break;

    Sleep(LOOP_DELAY_MS);
  }

  gotoxy(0, 12);
  if (collected > 0) {
    std::cout << "                             \n";
    std::cout << "   " << collected << " cross(es) collected! Bonus: "
              << collected * DAMAGE_PERCENT_BONUS << "%\n";
    std::cout << "                             \n";
  } else {
    std::cout << "                             \n";
    std::cout << "           MISS!              \n";
    std::cout << "                             \n";
  }
  Sleep(1500);
  system("cls");
  ShowConsoleCursor(true);
  return collected;
}
