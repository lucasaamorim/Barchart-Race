#pragma once

#include <fstream>
#include <vector>
#include "barchart.h"
#include "libs/coms.h"

//TODO: Implementar métodos
class FileParser {
  string file_path;
  vector<Frame> parsed_frames;
  Logger::SourceContext source_context; //!< Saves in which file and on which line the error occurred.
  string error_msg;
  //std::fstream file_logger;

  public:
  FileParser(string f_path) : file_path(f_path) {};
  void loadFile();
  void readHeader(std::ifstream& file);
  void fillingHeaderFrame(Frame& frame);
  int validateNumbersBarsForFrame(string& line);
  void processBarItens(int n_bars, std::ifstream& file, std::queue<string>& buffer);
  //void validateNumbersBarItens(string& line, bool& disrupted, std::ifstream& file);
  std::vector<Frame> getParsedFrames() const { return parsed_frames; }
  bool validateBarItens();

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