#include "CsvParser.h"

#include <fstream>
#include <sstream>

std::vector<std::vector<std::string>> CsvParser::parseFile(
    const std::string& filename, char delimiter) {
  std::vector<std::vector<std::string>> result;

  std::ifstream file(filename);

  if (!file.is_open()) return result;

  std::string line;

  while (std::getline(file, line)) {
    std::vector<std::string> row;

    std::stringstream ss(line);

    std::string cell;

    while (std::getline(ss, cell, delimiter)) {
      row.push_back(cell);
    }

    result.push_back(row);
  }

  return result;
}
