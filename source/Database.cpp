#include "Database.h"


namespace bcra {

  void Database::init(const std::string &title,const std::string &label,const std::string &src) {
    main_title = title;
    value_label = label;
    source = src;
  }

  void Database::add_category(std::string category, short color) {
    if (color_category.size() == Colors::COLORS.size()) {
      category_overflow = true;
      return;
    }
    color_category.insert({category, color});
  }

  void Database::add_BarItem(std::string &timestamp, std::string &label, value_t &value, std::string &category) {
    // Use a read/write reference to add to the chart that is currently being processed.
    // And consequently, the last chart in the vector at the moment.
    auto &bch = m_charts.back();
    bch.add_BarItem(label, value, category);
    bch.change_timestamp(timestamp);
  }

  void Database::sort_all() {
    // Calls for each of the Charts built-in sorting.
    for (auto &chart: m_charts) {
      chart.sort();
    }
  }

  void Database::show(size_t index, size_t max_bars) {
    auto &curr_chart = m_charts[index];
    // Graph Header.
    std::cout << "\t\t" << TextFormat::applyFormat(main_title, Colors::BLUE, Modifiers::BOLD) << "\n\n";
    std::cout << "\t\t\t\t" << TextFormat::applyFormat("Time Stamp: "+curr_chart.get_timestamp(), Colors::BLUE, Modifiers::BOLD) << "\n\n";

    //Displaying the bars.
    size_t num_bars = std::min(max_bars, curr_chart.get_n_bars());
    curr_chart.display_bars(num_bars, color_category,category_overflow);
    // Displaying the x_axis.
    show_x_axis(curr_chart, num_bars);
    // Value label below x-axis.
    cout << TextFormat::applyFormat(value_label, Colors::YELLOW, Modifiers::BOLD) << "\n\n";
    // Source for the Data in the Graph.
    cout << TextFormat::applyFormat(source, Colors::BRIGHT_WHITE, Modifiers::BOLD) << '\n';

    if (!category_overflow) { // If the Graph has color-coding, give it a caption.
      for (auto &i : color_category) {
      std::cout << TextFormat::applyFormat("   ", i.second, Modifiers::REVERSE) << TextFormat::applyFormat(": "+i.first, i.second, Modifiers::BOLD) << " ";
      }
    }
    cout << '\n';
  }

  void Database::show_x_axis(Frame &curr_chart,size_t num_bars){
    value_t first_nun_axis = curr_chart.get_value_m_BarItem(num_bars-1); // First x-axis value after zero (can be zero depending on bar values).
    value_t last_num_axis = curr_chart.get_value_m_BarItem(0); // Last x-axis value.
    int aux_first{0}, aux_last{0}; // Auxiliary variables.
    bool rest_dif_zero = false;
    // Rounds the first x-axis value after zero.
    while(first_nun_axis/10 != 0){
      first_nun_axis /= 10;
      aux_first++;
    }
    first_nun_axis *= std::pow(10, aux_first);
    // Rounds the last x-axis value.
    if(last_num_axis < 10){
      // last_num_axis does not change.
    }else if(last_num_axis < 100){
      while(last_num_axis/10 != 0){
        if(last_num_axis%10 != 0){
          rest_dif_zero = true;
        }
        last_num_axis /= 10;
        aux_last++;
      }
    }else{
      while(last_num_axis/10 >= 10){
        if(last_num_axis%10 != 0){
          rest_dif_zero = true;
        }
        last_num_axis /= 10;
        aux_last++;
      }
    }
    if(rest_dif_zero){
      last_num_axis++;
    }
    last_num_axis *= std::pow(10, aux_last);
    // Displaying the x-axis.
    value_t interval_values{(last_num_axis-first_nun_axis)/Cfg::n_ticks}; // Interval between each point on the x-axis
    size_t point_axis{0}; // X-axis points.
    size_t temp{1}; // Auxiliary variable.
    // Displaying x-axis arrow.
    cout << TextFormat::applyFormat("+",Colors::BRIGHT_WHITE,Modifiers::BOLD);
    for(value_t i{0};i<=Cfg::n_ticks;i++){
      auto val = curr_chart.get_value_m_BarItem(0);
      if (val != 0) point_axis = (Cfg::max_bar_length*(first_nun_axis+i*interval_values)/val);
      if(point_axis == 0) continue;
      for(size_t j{temp};j<point_axis-1;j++){
        cout << TextFormat::applyFormat("-",Colors::BRIGHT_WHITE,Modifiers::BOLD);
      }
      cout << TextFormat::applyFormat("+",Colors::BRIGHT_WHITE,Modifiers::BOLD);
      temp = point_axis;
    }
    for(size_t j{temp};j<Cfg::axis_size;j++){
      cout << TextFormat::applyFormat("-",Colors::BRIGHT_WHITE,Modifiers::BOLD);
    }
    cout << TextFormat::applyFormat(">",Colors::BRIGHT_WHITE,Modifiers::BOLD) << "\n";
    temp = 1;
    // Displaying x-axis values.
    string num_axis;
    cout << TextFormat::applyFormat("0",Colors::BRIGHT_YELLOW,Modifiers::BOLD);
    for(int i{0};i<=Cfg::n_ticks;i++){
      auto val = curr_chart.get_value_m_BarItem(0);
      if (val != 0) point_axis = (Cfg::max_bar_length*(first_nun_axis+i*interval_values)/val);
      if(point_axis == 0) continue;
      for(size_t j{temp};j<point_axis-1;j++){
        cout << " ";
      }
      // Ensures that the last value on the x-axis is rounded.
      if(i==Cfg::n_ticks){
        cout << TextFormat::applyFormat(std::to_string(last_num_axis),Colors::BRIGHT_YELLOW,Modifiers::BOLD);
      }else{
        num_axis = std::to_string(first_nun_axis+i*interval_values);
        temp = point_axis+num_axis.size()-1;
        cout << TextFormat::applyFormat(num_axis,Colors::BRIGHT_YELLOW,Modifiers::BOLD);
      }
    }
    cout << "\n";
  }
};
