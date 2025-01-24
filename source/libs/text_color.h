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
using color_t = short;
using mod_t = short;

/// @brief ANSI escape codes for text colors.
struct Colors {
  static constexpr color_t BLACK{30};
  static constexpr color_t RED{31};
  static constexpr color_t GREEN{32};
  static constexpr color_t YELLOW{33};
  static constexpr color_t BLUE{34};
  static constexpr color_t MAGENTA{35};
  static constexpr color_t CYAN{36};
  static constexpr color_t WHITE{37};
  static constexpr color_t BRIGHT_BLACK{90};
  static constexpr color_t BRIGHT_RED{91};
  static constexpr color_t BRIGHT_GREEN{92};
  static constexpr color_t BRIGHT_YELLOW{93};
  static constexpr color_t BRIGHT_BLUE{94};
  static constexpr color_t BRIGHT_MAGENTA{95};
  static constexpr color_t BRIGHT_CYAN{96};
  static constexpr color_t BRIGHT_WHITE{97};

  static constexpr array<color_t,16> COLORS{BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
                                          BRIGHT_BLACK, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW, BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE};
};

/// @brief ANSI escape codes for text modifiers. 
struct Modifiers {
  static constexpr mod_t REGULAR{0};
  static constexpr mod_t BOLD{1};
  static constexpr mod_t UNDERLINE{4};
  static constexpr mod_t BLINK{5};
  static constexpr mod_t REVERSE{7};

  static constexpr array<mod_t,5> MODIFIERS{REGULAR, BOLD, UNDERLINE, BLINK, REVERSE};
};

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
