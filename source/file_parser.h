#pragma once

#include <fstream>      // std::ifstream
#include <queue>        // std::queue
#include <memory>       // std::unique_ptr, std::shared_ptr
#include "barchart.h"   // Frame, Bar
#include "animation.h"  // AnimationManager
#include "libs/coms.h"  // Logger

/// @brief Metadata of the Charts 
struct Metadata {
  string main_title;
  string x_label;
  string source;
};

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
  std::shared_ptr<AnimationManager> animation_manager;  ///< Pointer to the AnimationManager instance

  public:
  typedef std::queue<string> buffer_t;

  FileParser(string f_path, std::shared_ptr<AnimationManager> am) : file_path(f_path), animation_manager(am) {};
  Metadata loadFile();
  void readFrame(std::ifstream& file, Frame& frame, int n_bars);
  string readBar(Bar& bar, buffer_t &tokens);
  
  buffer_t tokenizeLine(string &line);
  //Wrapper for std::getline that increments the line number in the source context
  std::istream& getline(std::ifstream &file, string &line) { 
    source_context.line++;
    return std::getline(file, line);
  };
};