#include "barchart.h"

/**
 * @brief Sorts the bars in descending order based on their values.
 * 
 * This function sorts the internal vector of bars using std::sort with a lambda 
 * comparator that compares bar values. Bars with higher values will appear first
 * in the sorted vector.
 * 
 * Time Complexity: O(n log n) where n is the number of bars
 */
void Frame::sortBars() {
  
  auto cmp = [] (const auto &a, const auto &b) {
    return a->getValue() > b->getValue();
  };

  std::sort(bars.begin(), bars.end(), cmp);
}

/**
 * @brief Calculates and updates the visual length of all bars in the frame
 * 
 * This function calculates the relative length of each bar based on its value
 * compared to the maximum value in the frame. The length is scaled proportionally
 * to fit within the specified bar_length.
 * 
 * The calculation uses the formula: length = (value * bar_length) / max_value
 * where max_value is the value of the highest bar in the frame.
 */
void Frame::calcLengths() {
  int max_value = bars.front()->getValue();

  for (auto &bar : bars) {
    bar->setLength(bar->getValue() * bar_length / max_value);
  }
}

/**
 * @brief Builds and returns a string representation of the X-axis for the bar chart
 * 
 * This function creates a horizontal axis with tick marks and corresponding numeric values.
 * The axis is scaled based on the maximum value of the bars and includes formatted numbers
 * (using K for thousands, M for millions, B for billions).
 * 
 * The axis consists of two parts:
 * 1. A line with '+' marks for ticks and '-' for the axis line, ending with '>'
 * 2. Numeric values aligned under each tick mark
 * 
 * @return string The formatted X-axis as a string, including tick marks and values
 * 
 * Example output:
 * +----+----+----+----+>
 * 0    25K  50K  75K  100K
 * 
 * @note The axis length and number of ticks are determined by class member variables
 *       axis_length and n_ticks
 * @note The scale is determined by the maximum value in the bars vector
 */
string Frame::buildXAxis() const {
  string axis;
  int tick_separation = (axis_length-1) / n_ticks;
  int max_value = bars.front()->getValue();

  
  auto number_format = [] (int number) {
    string formatted_number = std::to_string(number);
    if (number >= (int)1e9) {
      formatted_number = std::to_string(number / 1000000000) + "B";
    } else if (number >= (int)1e6) {
      if (number < (int)1e7) formatted_number = std::to_string(number / 1000000) + "." + std::to_string((number / 10000)%100) + "M";
      else formatted_number = std::to_string(number / 1000000) + "M";
    } else if (number >= (int)1e3) {
      if (number < (int)1e4) formatted_number = std::to_string(number / 1000) + "." + std::to_string((number / 10)%100) + "K";
      else formatted_number = std::to_string(number / 1000) + "K";
    }
    return formatted_number;
  };

  vector<int> tick_locations;
  vector<int> tick_values;
  tick_values.reserve(n_ticks), tick_locations.reserve(n_ticks);
  // Printing the Axis itself
  axis += "+"; //Put the first tick on zero.
  tick_locations.push_back(0);
  tick_values.push_back(0);
  for (int i = 1; i < axis_length; i++) {
    if (i % tick_separation == 0) {
      axis += "+";
      tick_locations.push_back(i);
      tick_values.push_back(i * max_value / axis_length);
    } else {
      axis += "-";
    }
  }

  axis += ">\n";

  // Printing the values
  int currpos = 0;
  for (int i = 0; i < tick_locations.size(); i++) {
    string tick_value_str = number_format(tick_values[i]);
    int space_count = std::max(0, tick_locations[i] - currpos);
    axis += std::string(space_count, ' ');
    axis += tick_value_str;
    currpos += space_count + tick_value_str.size();
  }
  axis += "\n";

  return axis;
}

/**
 * @brief Renders a bar with the specified color and label
 * 
 * This function creates and outputs a bar visualization by:
 * 1. Creating a string of spaces with length specified by the Bar's length
 * 2. Applying reverse formatting with the specified color
 * 3. Appending the bar's label with the same color
 * 4. Outputting the formatted bar to standard output with a newline
 *
 * @param color The color to use for both the bar and label
 */
string Bar::render(color_t color) const {
  std::stringstream ss;
  string bar = string(length, ' ');
  ss   << TextFormat::applyFormat(bar, color, Modifiers::REVERSE)
       << TextFormat::applyFormat(label, color) << TextFormat::applyFormat(" [", color) 
       << TextFormat::applyFormat(std::to_string(value), color) << TextFormat::applyFormat("]", color) << "\n";
  return ss.str();
}

/**
 * @brief Renders a frame of the bar chart to standard output
 * 
 * This overload renders all bars in the same color (cyan).
 * The frame includes a header with title and timestamp, the bars themselves,
 * an x-axis, and footer with x-axis label and source information.
 * 
 * @param n_bars The maximum number of bars to render. If n_bars is greater than
 *              the actual number of bars, all bars will be rendered
 * 
 * @throws RuntimeError if the frame is empty (logged via Logger::logError1)
 * 
 * @note Bars are sorted before rendering
 * @note All bars are rendered in cyan color
 */
string Frame::render(int n_bars) {
  if (empty()) {
    Logger::logError1("Cannot render an empty frame.");
    return "";
  }
  std::stringstream ss;
  // Chart Header
  ss << "\t\t" << TextFormat::applyFormat(title, Colors::BLUE, Modifiers::BOLD) << "\n\n";
  ss << "\t" << TextFormat::applyFormat("Time Stamp: "+timestamp, Colors::BLUE, Modifiers::BOLD) << "\n\n";

  //Chart Body
  sortBars();

  calcLengths();
  
  for (int i = 0; i < n_bars and i < bars.size(); i++) {
    ss << bars[i]->render(Colors::CYAN);
  }

  ss << buildXAxis();

  // Chart Footer
  ss << TextFormat::applyFormat(x_label, Colors::YELLOW, Modifiers::BOLD) << "\n\n";
  ss << TextFormat::applyFormat(source, Colors::WHITE, Modifiers::BOLD) << "\n";
  return ss.str();
}

/**
 * @brief Renders a frame of the bar chart race, coloring bars based on their categories
 * 
 * This method displays the complete frame including header (title and timestamp),
 * the bars themselves, x-axis, footer (x-label and source), and a color caption.
 * Bars are colored according to their category using the provided category-color mapping.
 * 
 * @param categories Map associating each category name with its corresponding color
 * @param n_bars Maximum number of bars to display (will display fewer if frame contains less bars)
 * 
 * @throws Logger::Error1 if the frame is empty
 * 
 * @note Bars are automatically sorted before rendering
 * @note If n_bars is greater than the actual number of bars, all bars will be displayed
 */
string Frame::render(std::map<string,color_t>& categories, int n_bars) {
  if (empty()) {
    Logger::logError1("Cannot render an empty frame.");
    return "";
  } else if (categories.size() > 15) {
    return render(n_bars);
  }

  std::stringstream ss;
  // Chart Header
  ss << "\t\t" << TextFormat::applyFormat(title, Colors::BLUE, Modifiers::BOLD) << "\n\n";
  ss << "\t" << TextFormat::applyFormat("Time Stamp: "+timestamp, Colors::BLUE, Modifiers::BOLD) << "\n\n";
  //Chart Body
  sortBars();

  calcLengths();

  for (int i = 0; i < n_bars and i < bars.size(); i++) {
    auto category_color = categories[bars[i]->getCategory()];
    ss << bars[i]->render(category_color);
  }
  
  ss << buildXAxis();
  // Chart Footer
  ss << TextFormat::applyFormat(x_label, Colors::YELLOW, Modifiers::BOLD) << "\n\n";
  ss << TextFormat::applyFormat(source, Colors::WHITE, Modifiers::BOLD) << "\n";
  // Color Caption
  for (const auto &[category_name, category_color] : categories) {
    ss << TextFormat::applyFormat("   ", category_color, Modifiers::REVERSE) 
         << TextFormat::applyFormat(": "+category_name, category_color, Modifiers::BOLD)
         << " ";
  }
  ss << '\n';
  return ss.str();
}