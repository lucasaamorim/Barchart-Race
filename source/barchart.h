#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <queue>
#include "libs/text_color.h"

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
    string getCategory() const { return category; }
  };

  std::priority_queue<Bar> bars; //Bars in descending order
  string title;
  string x_label;
  string timestamp;
  string source;

  public:
  void render(int n_bars); //Has > 15 categories or none
  void render(std::map<string,color_t> categories, int n_bars); //Has between 1 and 15 categories
  void addBar(const Bar &bar) { bars.push(bar); }
  void setTitle(const string &title) { this->title = title; }
  void setXLabel(const string &x_label) { this->x_label = x_label; }
  void setSource(const string &source) { this->source = source; }
  void setTimestamp(const string &timestamp) { this->timestamp = timestamp; }
};
