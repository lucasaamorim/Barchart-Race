/*!
 * Bar Char Race project.
 *
 * @author Selan
 * @data October 5th, 2021.
 */

#include "barchart.h"

namespace bcra {

  void Frame::add_BarItem(string &label, value_t &value, string &category) {
    m_bars.emplace_back(label, value, category);
  }

  void Frame::display_bars(const size_t &num_bars, std::map<string, short> &color_category,
                              const bool &overflow) {
    // Build each bar that will be displayed and print them.
    for (size_t i = 0; i < num_bars; i++) {
      auto &curr_bar = m_bars[i];
      // Note that for the largest bar bar_size will be MAX_SIZE_BAR.
      size_t bar_size{0};
      if (curr_bar.value != 0 && m_bars[0].value != 0) bar_size = (Cfg::max_bar_length*curr_bar.value)/m_bars[0].value;
      // Use the string constructor so that you don't need to use a for loop to print individual spaces.
      string bar(bar_size, ' ');
      // If an overflow has not happened you can print colorful bars.
      if (!overflow) {
        std::cout << TextFormat::applyFormat(bar, color_category[curr_bar.category], Modifiers::REVERSE);
        std::cout << TextFormat::applyFormat(" "+curr_bar.label+" ["+std::to_string(curr_bar.value)+"]\n\n", color_category[curr_bar.category], Modifiers::BOLD);
      } else { // Print all of them in the same Color.
        std::cout << TextFormat::applyFormat(bar, Cfg::default_color, Modifiers::REVERSE);
        std::cout << TextFormat::applyFormat(" "+curr_bar.label+" ["+std::to_string(curr_bar.value)+"]\n\n", Cfg::default_color, Modifiers::BOLD);
      }
    }
  }
}
