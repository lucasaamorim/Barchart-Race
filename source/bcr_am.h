#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>
#include <memory>
#include <thread>
#include <vector>
#include <map>
#include <filesystem>
#include <iterator>
#include <sstream>
#include <queue>
#include <numeric>
#include <tuple>
#include <string>
#include "libs/text_color.h"
#include "barchart.h"
#include "Database.h"
#include "libs/coms.h"

using std::vector;
using std::ostringstream;
using std::string;

namespace bcra {

  // Foward Declaration.
  class Database;

  /// Print out the usage instructions.
  void usage();

  /*!
   * Tokenizes values in a line.
   * @param line Line to tokenize.
   * @param buffer Buffer to push the tokens to.
  */
  size_t tokenize_line(string &line, std::queue<string> &buffer);

  /*!
   * Stops the program to match fps required by the User.
   * @param fps Number of graphs per second.
  */ 
  void break_time(const int &fps);

  /// Custom implementation of getline with some conviniences, similar to getline(strean >> std::ws, line).
  /*!
   * @param stream Stream to retrieve data from.
   * @param line Reference to string that will recieve the line.
   * @param line_cnt An integer to be incremented for every empty line it jumps over.
   * @return The input stream. It may have the failbit flag set on if it's empty.
   */
  std::istream& get_line(std::istream &stream, string &line, int &line_cnt);


  ///Holds some default values.
  struct Cfg {
    [[maybe_unused]]static constexpr short default_bars = 5; //!< Default value for # of bars.
    [[maybe_unused]]static constexpr short max_bars = 15;    //!< Max # of bars in the animation.
    [[maybe_unused]]static constexpr short min_bars = 1;     //!< Min # of bars in the animation.
    [[maybe_unused]]static constexpr short default_fps = 12; //!< Default fps.
    [[maybe_unused]]static constexpr short max_fps = 24;     //!< Max fps allowed.
    [[maybe_unused]]static constexpr short min_fps = 1;      //!< Min fps allowed.

    [[maybe_unused]]static constexpr short max_bar_length = 50; //!< Max bar length in characters.
    [[maybe_unused]]static constexpr short axis_size = 80; //!< The size of the Graph's X axis.
    [[maybe_unused]]static constexpr short n_ticks = 5; //!< Number of ticks on the X axis.
    [[maybe_unused]]static constexpr short height = 30; //!< How many lines total one screen have.
    [[maybe_unused]]static constexpr short width = 60;  //!< Total columns per screen.
    [[maybe_unused]]static constexpr short default_color = Colors::GREEN;
  };

//TODO: Break this class down into smaller ones.
/// Class representing an animation manager
  class AnimationManager {
  private:
    //=== Nested types

    //!< The animation states (Finite State Machine).
    enum class ani_state_e : uint {
      START = 0,     //!< Initial animation state.
      END,           //!< Final animation state.
      ERROR,         //!< Error mode.
      WELCOME,       //!< Initial message.
      READING_INPUT, //!< Reading input file.
      RACING         //!< Displaying bar chart race.
    };

    enum class opt_e : uint {
      MAX_BARS = 0, //!< Maximum number of bars to use.
      FPS           //!< Framerate option.
    };

    std::map<string, opt_e> opt_mp{
        {"-b", opt_e::MAX_BARS},
        {"-f", opt_e::FPS}
    };

    enum class read_status_e : uint {
      NOT_READ = 0, //!< File has no been read yet.
      ERROR,        //!< An error happened while reading the file.
      OK            //!< File has been read with no problems.
    };

    /// Types of errors that can happen.
    enum class error_e : uint {
        WARNING1 = 0,
        WARNING2,
        ERROR1,
        ERROR2,
        MESSAGE
    };

    /// Internal animation options
    struct Options {
      string input_filename; //!< Input data file.
      short n_bars;               //!< Requested # of bars per chart.
      short fps;                  //!< Animation speed in frames per second.
    };

    //=== Data members
    Options m_opt;    //!< Overall options to set up the animation configuration.
    ani_state_e m_animation_state; //!< Current animation state.
    Database* m_db; //!< The different Charts that make up the animation.
    read_status_e m_read_status; //!< File reading status.
    Logger::SourceContext source_context; //!< Saves in which file and on which line the error occurred.
    vector<std::tuple<error_e,string,Logger::SourceContext>> m_error_msgs;
    int curr_frame; //!< Current frame index.

  public:
    AnimationManager();

    AnimationManager(const AnimationManager &_clone) = delete;

    AnimationManager(AnimationManager &&) = delete;

    AnimationManager &operator=(const AnimationManager &_rhs) = delete;

    AnimationManager &operator=(AnimationManager &&) = delete;

    ~AnimationManager() = default;

    //=== Common methods for the animation Loop design pattern.
    void initialize(int, char **);

    /// Renders the animation to the user.
    void render();

    /// Update the animation based on the current animation state.
    void update();

    /// Process user input events, depending on the current animation state.
    void process_events();

    /// Returns true when the user wants to quit the animation.
    [[nodiscard]] bool is_over() const;

  private:
    /// Reads a file and Builds the Chart objects.
    read_status_e read_file();

  }; // BCRAnimation class.
} // namespace bcra
