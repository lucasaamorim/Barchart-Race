#pragma once

#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include "barchart.h"
#include "libs/coms.h"

//TODO: Implementar métodos
class FileParser {
  string file_path;
  vector<Frame> parsed_frames;
  Logger::SourceContext source_context; //!< Saves in which file and on which line the error occurred.
  std::set<string> categories;

  public:
  FileParser(string f_path) : file_path(f_path) {};
  void loadFile();
  void readHeader(std::ifstream& file);
  void fillFrameHeader(Frame& frame);
  int validateNumbersBarsForFrame(string& line);
  void processData(int n_bars, std::ifstream& file, std::queue<string>& buffer, Frame& frame);
  //void validateNumbersBarItens(string& line, bool& disrupted, std::ifstream& file);
  bool setBarItens(std::queue<string>& buffer, Bar& bar);
  bool validateBarValue(string& item, int& value);
  std::vector<Frame> getParsedFrames() const { return parsed_frames; }

  /// Custom implementation of getline with some conviniences, similar to getline(strean >> std::ws, line).
  /*!
   * @param stream Stream to retrieve data from.
   * @param line Reference to string that will recieve the line.
   * @param line_cnt An integer to be incremented for every empty line it jumps over.
   * @return The input stream. It may have the failbit flag set on if it's empty.
   */
  std::istream& get_line(std::istream &stream, string &line, int &line_cnt);

  /*!
   * Tokenizes values in a line.
   * @param line Line to tokenize.
   * @param buffer Buffer to push the tokens to.
  */
  size_t tokenize_line(string &line, std::queue<string> &buffer);
};