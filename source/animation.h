#pragma once

#include <thread>
#include <chrono>
#include "barchart.h"
#include "renderer.h"

//TODO: Implementar métodos
class AnimationManager {
  vector<Frame> frames;
  TerminalRenderer renderer;
  int frame_rate;

  public:
    void addFrame(const Frame &frame) { frames.emplace_back(frame); }
    void PlayAnimation(int fps);
    void setFrameRate(int fps) { frame_rate = fps; }
    //void smoothFrames(); would be cool but will not implement it right now
};