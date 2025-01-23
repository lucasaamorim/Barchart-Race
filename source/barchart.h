#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "libs/text_color.h"

using std::string;
using std::vector;
using std::cout;

class Frame {
  using color_t = short;
  class Bar {
    int length;
    int value;
    color_t color;
    string label;

    public:
    inline bool operator<(const Bar &B) const { return this->value < B.value; }
    inline bool operator>(const Bar &B) const { return B < *this; }
    inline bool operator<=(const Bar &B) const { return !(*this > B); }
    inline bool operator>=(const Bar &B) const { return !(*this < B); }
    void render();
    void setLength(const int &length) { this->length = length; }
    void setValue(const int &value) { this->value = value; }
    void setColor(const color_t &color) { this->color = color; }
  };

  vector<Bar> bars;
  string title;
  string x_label;
  string y_label;
  string timestamp;

  public:
  void render();
  void addBar(const Bar &bar) { bars.emplace_back(bar); }
  void setTitle(const string &title) { this->title = title; }
  void setXLabel(const string &x_label) { this->x_label = x_label; }
  void setYLabel(const string &y_label) { this->y_label = y_label; }
  void setTimestamp(const string &timestamp) { this->timestamp = timestamp; }
};
