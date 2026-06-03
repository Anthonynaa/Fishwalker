#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <vector>

class CsvParser {
 public:
  static std::vector<std::vector<std::string>> parseFile(
      const std::string& filename, char delimiter = ';');
};

#endif
