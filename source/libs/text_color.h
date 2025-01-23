#pragma once
/*!
 * Color code:
 * https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
 * 
 * How to use this function:
 * ```c++
 *      std::cout << "Esse texto aparece normal, "
 *                << Color::tcolor("texto em vermelho e negrito", Colors::RED, Modifiers::BOLD)
 *                << "\n";
 * ```
 */
#include <sstream>
#include <string>
#include <array>

using std::ostringstream;
using std::string;
using std::array;

class TextFormat {
public:
  /// Returns a string with a colored message.
  /*!
   * @param msg Message to display.
   * @param color Color code to apply to the message.
   * @param modifier Modifier code to apply to the message.
   * @return A string with the embedded color/modifier escape codes.
   */
  static std::string applyFormat(const std::string &msg, short color = Colors::WHITE, short modifier = Modifiers::REGULAR) {
    ostringstream oss;
    oss << "\33[" << modifier << ";" << color << "m" << msg << "\33[0m";
    return oss.str();
  }
};

/// @brief ANSI escape codes for text colors.
struct Colors {
  static constexpr short BLACK{30};
  static constexpr short RED{31};
  static constexpr short GREEN{32};
  static constexpr short YELLOW{33};
  static constexpr short BLUE{34};
  static constexpr short MAGENTA{35};
  static constexpr short CYAN{36};
  static constexpr short WHITE{37};
  static constexpr short BRIGHT_BLACK{90};
  static constexpr short BRIGHT_RED{91};
  static constexpr short BRIGHT_GREEN{92};
  static constexpr short BRIGHT_YELLOW{93};
  static constexpr short BRIGHT_BLUE{94};
  static constexpr short BRIGHT_MAGENTA{95};
  static constexpr short BRIGHT_CYAN{96};
  static constexpr short BRIGHT_WHITE{97};

  static constexpr array<short,16> COLORS{BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
                                          BRIGHT_BLACK, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW, BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE};
};

/// @brief ANSI escape codes for text modifiers. 
struct Modifiers {
  static constexpr short REGULAR{0};
  static constexpr short BOLD{1};
  static constexpr short UNDERLINE{4};
  static constexpr short BLINK{5};
  static constexpr short REVERSE{7};

  static constexpr array<short,5> MODIFIERS{REGULAR, BOLD, UNDERLINE, BLINK, REVERSE};
};
