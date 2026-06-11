#include "ConsoleUI.h"

#include <cstdlib>
#include <iostream>
#include <limits>

void ConsoleUI::PrintHeader(const std::string& title) {
  system("cls");
  std::cout << "\n=========================\n";
  std::cout << title << "\n";
  std::cout << "=========================\n";
}

int ConsoleUI::ShowMenu(const std::string& title,
                        const std::vector<std::string>& options) {
  PrintHeader(title);
  for (size_t i = 0; i < options.size(); ++i) {
    std::cout << i + 1 << ". " << options[i] << "\n";
  }
  std::cout << "\n0. Back\n";
  std::cout << "> ";
  int choice = ConsoleUI::ReadInt();
  return choice;
}

void ConsoleUI::Pause() {
  std::cout << "\nPress Enter...";
  std::cin.ignore();
  std::cin.get();
}

int ConsoleUI::ReadInt() {
  int value;
  while (!(std::cin >> value)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input. Try again:\n> ";
  }
  return value;
}

void ConsoleUI::ShowVictoryScreen() {
  system("cls");

  std::cout << "\n";
  std::cout << "========================================\n";
  std::cout << "              CONGRATULATIONS\n";
  std::cout << "========================================\n\n";

  std::cout << "The source of the infection has been defeated.\n";
  std::cout << "The fate of the coast is now in your hands.\n\n";

  std::cout << "Thank you for playing Fishwalker!\n";

  std::cout << "\n========================================\n";
  std::cout << "\nPress Enter to exit...";
}

int ConsoleUI::ShowStartMenu() {
  system("cls");

  std::cout << "========================================\n";
  std::cout << "              FISHWALKER\n";
  std::cout << "========================================\n";
  std::cout << "Text-RPG by Roman Sisoev and Anton Novikov\n";
  std::cout << "\n";
  std::cout << "1. Start Game\n";
  std::cout << "0. Exit\n";
  std::cout << "\n> ";

  int choice;
  std::cin >> choice;

  return choice;
}
