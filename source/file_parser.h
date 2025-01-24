#pragma once

#include <fstream>      // std::ifstream
#include <vector>       // std::vector
#include <queue>        // std::queue
#include <set>          // std::set
#include <memory>       // std::unique_ptr, std::shared_ptr
#include "barchart.h"   // Frame, Bar
#include "animation.h"  // AnimationManager
#include "libs/coms.h"  // Logger

class FileParser {
  string file_path;
  Logger::SourceContext source_context; //!< Saves in which file and on which line the error occurred.
  std::set<string> categories;
  std::shared_ptr<AnimationManager> animation_manager;

  public:
  FileParser(string f_path, std::shared_ptr<AnimationManager> am) : file_path(f_path), animation_manager(am) {};
  void loadFile();
  void readHeader(std::ifstream& file);
  void fillFrameHeader(std::unique_ptr<Frame>& frame);
  int validateNumbersBarsForFrame(string& line);
  void processData(int n_bars, std::ifstream& file, std::queue<string>& buffer, std::unique_ptr<Frame>& frame);
  //void validateNumbersBarItens(string& line, bool& disrupted, std::ifstream& file);
  bool setBarItens(std::queue<string>& buffer, std::unique_ptr<Bar>& bar);
  bool validateBarValue(string& item, int& value);

  /// Custom implementation of getline with some conviniences, similar to getline(strean >> std::ws, line).
  std::istream& get_line(std::istream &stream, string &line, int &line_cnt);

  size_t tokenize_line(string &line, std::queue<string> &buffer);
};