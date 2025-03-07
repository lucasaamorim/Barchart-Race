#pragma once

#include <thread>       // std::this_thread::sleep_for
#include <chrono>       // std::chrono::milliseconds
#include <memory>       // std::unique_ptr
#include <iostream>     // std::cout
#include <map>          // std::map
#include <string>       // std::string
#include <vector>       // std::vector
#include "barchart.h"   // Frame

using std::cout;

class AnimationManager {
  vector<std::unique_ptr<Frame>> frames;  ///< Vector of frames
  std::map<string, color_t> categories;   ///< Map of categories and colors

  public:
    AnimationManager() = default;

    void addFrame(std::unique_ptr<Frame> frame) { frames.push_back(std::move(frame)); }
    void addCategoryColor(string category) { 
      if (categories.find(category) == categories.end()) {
        categories[category] = Colors::COLORS[categories.size()%Colors::COLORS.size()]; 
      }
    }
    void PlayAnimation(int fps, int n_bars);
    size_t numberCharts() { return frames.size(); }
    size_t numberCategories() { return categories.size(); }
    //void smoothFrames(); would be cool but will not implement it right now
};