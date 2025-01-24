#pragma once

#include <thread>       // std::this_thread::sleep_for
#include <chrono>       // std::chrono::milliseconds
#include <memory>       // std::unique_ptr
#include <map>          // std::map
#include <string>       // std::string
#include <vector>       // std::vector
#include "barchart.h"   // Frame

class AnimationManager {
  vector<std::unique_ptr<Frame>> frames;
  std::map<string, color_t> categories;

  public:
    AnimationManager() = default;

    void addFrame(std::unique_ptr<Frame> frame) { frames.push_back(std::move(frame)); }
    void addCategoryColor(string category) { categories[category] = Colors::COLORS[categories.size()%Colors::COLORS.size()]; }
    void PlayAnimation(int fps, int n_bars);
    //void smoothFrames(); would be cool but will not implement it right now
};