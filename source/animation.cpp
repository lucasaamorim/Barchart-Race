#include "animation.h"

void AnimationManager::PlayAnimation(int fps, int n_bars) {
  // Save cursor position
  cout << "\033[s";
  for (auto &frame : frames) {
    // Restore cursor position & clear screen
    cout << "\033[u\033[J";
    if (categories.empty() or categories.size() > 15) frame->render(n_bars);
    else frame->render(categories,n_bars);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
  }
}