#ifndef COMS_H
#define COMS_H

#include <iostream>
#include <iomanip> // setw()
#include <string>
#include <sstream>

#include "text_color.h"

#define SC SourceContext(__FILE__, __LINE__)

#define ERROR(msg) Error( msg, SC )
#define WARNING(msg) Warning( msg, SC )
#define MESSAGE(msg) Message( msg )

namespace coms {
  // Holds context information for a warning or error while pre-processing scene file or executing the renderer.
  // Use macro SC to create one
  struct SourceContext {
    string file;
    int line;

    SourceContext(string file = "n/a", int line = 0) : file(file), line(line) {}

    SourceContext &operator=(const SourceContext &) = default;

    SourceContext(const SourceContext &clone) = default;

    ~SourceContext() = default;
  };

  void Error2(const std::string &, const SourceContext &);

  void Error1(const std::string &);

  void Warning2(const std::string &, const SourceContext &);

  void Warning1(const std::string &);

  void Message(const std::string &);
} // namespace coms

#endif
//-------------------------------------------------------------------------------
