#pragma once

#include <fstream>      // std::ifstream
#include <vector>       // std::vector
#include <queue>        // std::queue
#include <set>          // std::set
#include <memory>       // std::unique_ptr, std::shared_ptr
#include "barchart.h"   // Frame, Bar
#include "animation.h"  // AnimationManager
#include "libs/coms.h"  // Logger

/**
 * @brief A class responsible for parsing input files containing bar chart animation data.
 * 
 * The FileParser class reads and validates input files containing data for bar chart race animations.
 * It processes the file header, metadata, and frame data, creating the necessary data structures
 * for the animation.
 */
class FileParser {
  string file_path;                                     ///< Path to the input file
  Logger::SourceContext source_context;                 ///< Logger source context for the FileParser class
  std::set<string> categories;                          ///< Set of unique categories found in the data
  std::shared_ptr<AnimationManager> animation_manager;  ///< Pointer to the AnimationManager instance

  /**
   * @brief Structure holding metadata information for the animation
   */
  struct Metadata {
    string main_title;  ///< Main title of the animation
    string x_label;     ///< Label for the x-axis
    string source;      ///< Source attribution for the data
  } metadata;

  public:
  FileParser(string f_path, std::shared_ptr<AnimationManager> am) : file_path(f_path), animation_manager(am) {};
  void loadFile();
  void readHeader(std::ifstream& file);
  void fillFrameMetadata(Frame& frame);
  int validateNumbersBarsForFrame(string& line);
  void processData(int n_bars, std::ifstream& file, std::queue<string>& buffer, Frame& frame);
  bool validateNumberBarItens(int& n_itens, bool& disrupted, std::queue<string>& buffer);
  bool setBarItens(std::queue<string>& buffer, Bar& bar);
  bool validateBarValue(string& item, int& value);
  void getMetadata(string& main_title, string& x_label, string& source);

  /// Custom implementation of getline with some conviniences, similar to getline(strean >> std::ws, line).
  std::istream& getLine(std::istream &stream, string &line, int &line_cnt);

  size_t tokenizeLine(string &line, std::queue<string> &buffer);
};