#pragma once

#include <string>             // std::string
#include <vector>             // std::vector
#include <iostream>           // std::cout
#include <map>                // std::map
#include <memory>             // std::shared_ptr
#include <algorithm>          // std::sort, std::max
#include "libs/text_color.h"  // color_t
#include "libs/coms.h"        // Logger 

using std::string;
using std::vector;
using std::cout;

/**
 * @brief A class representing a single bar in the bar chart
 * 
 * Contains the bar's value, display length, label and category information,
 * along with comparison operators for sorting.
 */
class Bar {
  int length;      ///< The display length of the bar in characters
  int value;       ///< The numeric value represented by the bar
  string label;    ///< The text label for the bar
  string category; ///< The category this bar belongs to

  public:
  inline bool operator<(const Bar &B) const { return this->value < B.value; }
  inline bool operator>(const Bar &B) const { return B < *this; }
  inline bool operator<=(const Bar &B) const { return !(*this > B); }
  inline bool operator>=(const Bar &B) const { return !(*this < B); }
  void render(color_t color) const;
  void setLength(const int &length) { this->length = length; }
  void setValue(const int &value) { this->value = value; }
  void setCategory(const string &category) { this->category = category; }
  void setLabel(const string &label) { this->label = label; }
  int getValue() const { return value; }
  string getCategory() const { return category; }
};

/**
 * @brief A class representing a single frame in a bar chart animation
 * 
 * The Frame class contains all necessary data to render a single frame of a bar chart,
 * including the bars themselves, title, labels, and sizing information.
 */
class Frame {
  std::vector<std::shared_ptr<Bar>> bars; ///< Collection of bars stored in descending order
  string title;                           ///< The title of the chart
  string x_label;                         ///< The label for the x-axis
  string timestamp;                       ///< The timestamp for this frame
  string source;                          ///< The data source information
  int bar_length;                         ///< Maximum number of characters a bar can occupy
  int axis_length;                        ///< Length of the x-axis in characters
  int n_ticks;                            ///< Number of tick marks on the x-axis

  public:
  void render(int n_bars); // Has > 15 categories or none
  void render(std::map<string,color_t> categories, int n_bars); // Has between 1 and 15 categories
  void calcLengths();
  void sortBars();
  void addBar(const Bar &bar) { bars.emplace_back(bar); }
  string buildXAxis() const;
  bool empty() { return bars.empty(); }

  void setTitle(const string &title) { this->title = title; }
  void setXLabel(const string &x_label) { this->x_label = x_label; }
  void setTimestamp(const string &timestamp) { this->timestamp = timestamp; }
  void setSource(const string &source) { this->source = source; }
};
