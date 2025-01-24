#include "animation.h"

/**
 * @brief Plays the animation frame by frame at the specified framerate
 * 
 * @param fps The frames per second at which to play the animation
 * @param n_bars The number of bars to display in each frame
 * 
 * @details The function iterates through all frames in the animation, rendering each one
 * and introducing a delay between frames to achieve the desired framerate.
 * If categories are defined and their number is 15 or less, it renders the frame with categories.
 * Otherwise, it renders the frame without category information.
 * 
 * The delay between frames is calculated as 1000/fps milliseconds.
 */
void AnimationManager::PlayAnimation(int fps, int n_bars) {
  for (auto &frame : frames) {
    if (categories.empty() or categories.size() > 15) frame->render(n_bars);
    else frame->render(categories,n_bars);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
  }
}