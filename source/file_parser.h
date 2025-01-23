#pragma once

#include <fstream>
#include <vector>
#include "barchart.h"
#include "libs/coms.h"

//TODO: Implementar métodos
class FileParser {
  string file_path;
  vector<Frame> parsed_frames;

  public:
  void loadFile(const string &file_path);
  void parseData();
  std::vector<Frame> getParsedFrames() const { return parsed_frames; }
  bool validateFile() const;
};