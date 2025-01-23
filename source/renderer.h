#pragma once

#include <string>

using std::string;

//TODO: Implementar métodos
/// Collection of render functions for different objects. 
struct TerminalRenderer {
  void clearScreen();
  void renderFrame(const Frame &frame);
  void renderText(const string &text);
};