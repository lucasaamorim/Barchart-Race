#include "barchart.h"

void Frame::Bar::render(color_t color) const {
  string bar = string(' ', length);
  bar = TextFormat::applyFormat(bar, color, Modifiers::REVERSE);
  bar += TextFormat::applyFormat(label, color);

  cout << bar << '\n';
}

void Frame::render(int n_bars) {
  // Chart Header
  cout << "\t\t" << TextFormat::applyFormat(title, Colors::BLUE, Modifiers::BOLD) << "\n\n";
  cout << "\t" << TextFormat::applyFormat("Time Stamp: "+timestamp, Colors::BLUE, Modifiers::BOLD) << "\n\n";

  //Chart Body
  for(int i = 0; i < n_bars and !bars.empty(); i++) {
    auto bar = bars.top();
    bar.render(Colors::CYAN);
    bars.pop();
  }

  //TODO: Render x axis
  
  // Chart Footer
  cout << TextFormat::applyFormat(x_label, Colors::YELLOW, Modifiers::BOLD) << "\n\n";
  cout << TextFormat::applyFormat(source, Colors::WHITE, Modifiers::BOLD) << "\n";
}

void Frame::render(std::map<string,color_t> categories, int n_bars) {
  // Chart Header
  cout << "\t\t" << TextFormat::applyFormat(title, Colors::BLUE, Modifiers::BOLD) << "\n\n";
  cout << "\t" << TextFormat::applyFormat("Time Stamp: "+timestamp, Colors::BLUE, Modifiers::BOLD) << "\n\n";

  //Chart Body
  for(int i = 0; i < n_bars and !bars.empty(); i++) {
    auto bar = bars.top();
    bar.render(categories[bar.getCategory()]);
    bars.pop();
  }
  //TODO: Render x axis

  // Chart Footer
  cout << TextFormat::applyFormat(x_label, Colors::YELLOW, Modifiers::BOLD) << "\n\n";
  cout << TextFormat::applyFormat(source, Colors::WHITE, Modifiers::BOLD) << "\n";

  for (const auto &[category_name, category_color] : categories) {
    cout << TextFormat::applyFormat("   ", category_color, Modifiers::REVERSE) 
         << TextFormat::applyFormat(": "+category_name, category_color, Modifiers::BOLD) << " ";;
  }
  cout << '\n';
}