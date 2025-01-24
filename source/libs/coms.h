#pragma once

#include <iostream>
#include <iomanip> // setw()
#include <string>
#include <sstream>
#include <fstream>

#include "text_color.h"

class Logger{
private:
  std::ofstream logger_file;
public:
  struct SourceContext
  {
    string file;
    int line;

    SourceContext(string file = "n/a", int line = 0) : file(file), line(line) {}
    SourceContext &operator=(const SourceContext &) = default;
    SourceContext(const SourceContext &clone) = default;
    ~SourceContext() = default;
  };

  void open_file();
  static void logError1(const std::string &message);
  static void logError2(const std::string &message, const SourceContext &sc);
  static void logWarning1(const std::string &message);
  static void logWarning2(const std::string &message, const SourceContext &sc);
  static void logDebug(const std::string &message);
};
