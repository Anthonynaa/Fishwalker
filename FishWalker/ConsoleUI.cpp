#include "ConsoleUI.h"

#include <iostream>

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

  int choice;
  std::cin >> choice;

  return choice;
}

void ConsoleUI::Pause() {
  std::cout << "\nPress Enter...";
  std::cin.ignore();
  std::cin.get();
}
