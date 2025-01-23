#include "animation.h"

void AnimationManager::PlayAnimation(int fps) {
  for (auto &frame : frames) {
    renderer.clearScreen();
    renderer.renderFrame(frame);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
  }
}