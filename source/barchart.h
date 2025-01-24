#pragma once

#include <string>             // std::string
#include <vector>             // std::vector
#include <iostream>           // std::cout
#include <map>                // std::map
#include <memory>             // std::shared_ptr
#include <algorithm>          // std::sort, std::max
#include "libs/text_color.h"  // color_t

using std::string;
using std::vector;
using std::cout;
class Frame {
  class Bar {
    int length;
    int value;
    string label;
    string category;

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

  std::vector<std::shared_ptr<Bar>> bars; //Bars in descending order
  string title;
  string x_label;
  string timestamp;
  string source;
  int bar_length; //upper bound to the number of space characters a bar can have
  int axis_length;
  int n_ticks;

  public:
  void render(int n_bars); //Has > 15 categories or none
  void render(std::map<string,color_t> categories, int n_bars); //Has between 1 and 15 categories
  void calcLengths();
  void sortBars();
  void addBar(const Bar &bar) { bars.emplace_back(bar); }
  string buildXAxis() const;

  void setTitle(const string &title) { this->title = title; }
  void setXLabel(const string &x_label) { this->x_label = x_label; }
  void setTimestamp(const string &timestamp) { this->timestamp = timestamp; }
  void setSource(const string &source) { this->source = source; }
};
