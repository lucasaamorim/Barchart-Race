#include "coms.h"

inline std::ostream &operator<<(std::ostream &os, const Logger::SourceContext &sc) {
  os << sc.file << ":" << sc.line << "ª linha";
  return os;
}

/// Prints out the warning, but the program keeps running.
void Logger::logWarning1(const std::string &msg) {
  std::cerr << std::endl << std::setw(80) << std::setfill('=') << " " << std::endl
            << "Warning: \"" << msg << "\"\n"
            << std::setw(80) << std::setfill('=') << " " << std::endl;
}

/// Prints out the warning + source context, but the program keeps running.
void Logger::logWarning2(const std::string &msg, const SourceContext &sc) {
  std::cerr << std::endl << std::setw(80) << std::setfill('=') << " " << std::endl
            << "Warning: \"" << msg << "\"\n"
            << "     REPORTED AT: < " << sc << " > \n"
            << std::setw(80) << std::setfill('=') << " " << std::endl;
}

/// Prints out the error message and exits the program.
void Logger::logError1(const std::string &msg) {
  std::cerr << std::endl << std::setw(80) << std::setfill('=') << " " << std::endl
            << "Severe error: " << TextFormat::applyFormat("\"" + msg + "\"", Colors::RED) << std::endl
            << "     exiting...\n"
            << std::setw(80) << std::setfill('=') << " " << std::endl;

  std::exit(EXIT_FAILURE);
}

/// Prints out the error message + source context, and exits the program.
void Logger::logError2(const std::string &msg, const SourceContext &sc) {
  std::cerr << std::endl << std::setw(80) << std::setfill('=') << " " << std::endl
            << "Severe error: " << TextFormat::applyFormat("\"" + msg + "\"", Colors::RED) << std::endl
            << "     REPORTED AT: < " << sc << " > " << std::endl
            << "     exiting...\n"
            << std::setw(80) << std::setfill('=') << " " << std::endl;

  std::exit(EXIT_FAILURE);
}

/// Prints out a regular message.
void Logger::logDebug(const std::string &str) {
  std::cout << TextFormat::applyFormat(">>> " + str, Colors::GREEN) << std::endl;
}
