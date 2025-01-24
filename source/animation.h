#pragma once

#include <thread>
#include <chrono>
#include "barchart.h"

//TODO: Implementar métodos
class AnimationManager {
  vector<Frame> frames;
  std::map<string, color_t> categories;
  int frame_rate;

  public:
    void addFrame(const Frame &frame) { frames.emplace_back(frame); }
    void PlayAnimation(int fps);
    void setFrameRate(int fps) { frame_rate = fps; }
    //void smoothFrames(); would be cool but will not implement it right now
};