#include "animation.h"

void AnimationManager::PlayAnimation(int fps) {
  // Save cursor position
  cout << "\033[s";
  for (auto &frame : frames) {
    // Restore cursor position & clear screen
    cout << "\033[u\033[J";
    //FIXME: Replace 5 with the actual number of bars to render
    if (categories.empty() or categories.size() > 15) frame->render(5);
    else frame->render(categories,5);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
  }
}