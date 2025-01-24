#include "barchart.h"

void Frame::sortBars() {
  
  auto cmp = [] (const auto &a, const auto &b) {
    return a->getValue() > b->getValue();
  };

  std::sort(bars.begin(), bars.end(), cmp);
}

void Frame::calcLengths() {
  int max_value = bars.back()->getValue();

  for (auto &bar : bars) {
    bar->setLength(bar->getValue() * bar_length / max_value);
  }
}

string Frame::buildXAxis() const {
  string axis;
  int tick_separation = (axis_length-1) / n_ticks;
  int max_value = bars.back()->getValue();

  
  auto number_format = [] (int number) {
    string formatted_number = std::to_string(number);
    if (number >= (int)1e9) {
      formatted_number = std::to_string(number / 1000000000) + "B";
    } else if (number >= (int)1e6) {
      formatted_number = std::to_string(number / 1000000) + "M";
    } else if (number >= (int)1e3) {
      formatted_number = std::to_string(number / 1000) + "K";
    }
    return formatted_number;
  };

  vector<int> tick_locations;
  vector<int> tick_values;
  // Printing the Axis itself
  axis += "+"; //Put the first tick on zero.
  tick_locations.push_back(0);
  for (int i = 1; i < axis_length; i++) {
    if (i % tick_separation == 0) {
      axis += "+";
      tick_locations.push_back(i);
      tick_values.push_back(i * max_value / axis_length);
    } else {
      axis += "-";
    }
  }

  axis += ">\n";
  // Printing the values

  for (int i = 0; i < tick_locations.size(); i++) {
    axis += std::string(tick_locations[i] - std::to_string(tick_values[i]).size(), ' ');
    axis += number_format(tick_values[i]) + "\n";
  }

  return axis;
}

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
  sortBars();
  for (int i = 0; i < n_bars and i < bars.size(); i++) {
    bars[i]->render(Colors::CYAN);
  }

  buildXAxis();

  // Chart Footer
  cout << TextFormat::applyFormat(x_label, Colors::YELLOW, Modifiers::BOLD) << "\n\n";
  cout << TextFormat::applyFormat(source, Colors::WHITE, Modifiers::BOLD) << "\n";
}

void Frame::render(std::map<string,color_t> categories, int n_bars) {
  // Chart Header
  cout << "\t\t" << TextFormat::applyFormat(title, Colors::BLUE, Modifiers::BOLD) << "\n\n";
  cout << "\t" << TextFormat::applyFormat("Time Stamp: "+timestamp, Colors::BLUE, Modifiers::BOLD) << "\n\n";

  //Chart Body
  sortBars();
  for (int i = 0; i < n_bars and i < bars.size(); i++) {
    bars[i]->render(categories[bars[i]->getCategory()]);
  }
  
  buildXAxis();

  // Chart Footer
  cout << TextFormat::applyFormat(x_label, Colors::YELLOW, Modifiers::BOLD) << "\n\n";
  cout << TextFormat::applyFormat(source, Colors::WHITE, Modifiers::BOLD) << "\n";
  // Color Caption
  for (const auto &[category_name, category_color] : categories) {
    cout << TextFormat::applyFormat("   ", category_color, Modifiers::REVERSE) 
         << TextFormat::applyFormat(": "+category_name, category_color, Modifiers::BOLD) << " ";;
  }
  cout << '\n';
}