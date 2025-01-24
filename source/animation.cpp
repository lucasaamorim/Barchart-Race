#include "animation.h"

void AnimationManager::PlayAnimation(int fps, int n_bars) {
  for (auto &frame : frames) {
    if (categories.empty() or categories.size() > 15) frame->render(n_bars);
    else frame->render(categories,n_bars);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
  }
}