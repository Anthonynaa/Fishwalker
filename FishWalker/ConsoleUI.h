#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <string>
#include <vector>

class ConsoleUI {
 public:
  static void PrintHeader(const std::string& title);

  static int ShowMenu(const std::string& title,
                      const std::vector<std::string>& options);

  static void Pause();

  static int ReadInt();
};

#endif
