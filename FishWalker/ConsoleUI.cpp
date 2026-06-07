#include "ConsoleUI.h"

#include <iostream>
#include <limits>

void ConsoleUI::PrintHeader(const std::string& title) {
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
