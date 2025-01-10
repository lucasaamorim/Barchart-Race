#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include "barchart.h"
#include "bcr_am.h"


namespace bcra {
  
  // Forward Declaration.
  class BarChart;

  using value_t = long;

  /// Database containing all the charts, title, colors and more.
  class Database {
    //=== Data members
    string main_title; //!< Main Title of the Graph.
    string source; //!< Label for the Graph's x-axis.
    string value_label; //!< Data Source.
    std::vector<BarChart> m_charts; //!< Charts that we got from the file.
    bool category_overflow; //!< Flag that indicates when there are too many categories to color-code.
    std::map<string, short> color_category; //!< Maps a category to a color, triggers category_overflow if there aren't enough colors.
  public:
    /// Default ctro.
    Database() = default;

    //===Methods

    /// Returns the Title.
    [[nodiscard]]string get_title() const { return main_title; }

    /// Returns the Value Label.
    [[nodiscard]]string get_value_label() const { return value_label; }

    /// Returns the Source.
    [[nodiscard]]string get_source() const { return source; }

    /// Returns the number of categories.
    [[nodiscard]]short get_n_categories() const { return color_category.size(); }

    /// Returns the number of Charts.
    [[nodiscard]]short get_n_charts() const { return m_charts.size(); }

    ///Initializes the basic elements for the DB (Title, Value Label and Source).
    void init(const string &title,const string &label,const string &src);

    /// Adds a category to the color_category and maps it to a color.
    /*!
     * @param category The category to be color-coded.
     * @param color The Color to be associated with the category.
     * @return None. It only inserts the key-value pair in color_category.
     */
    void add_category(string category, short color);

    /// Searches for a category and returns true if it's already on color_category.
    /*!
     * @param category The category to search.
     * @return True if the category is in color_category.
     */
    bool find_category(const string &category) { return color_category.find(category) != color_category.end(); }

    /// Builds a chart that will have bars and timestamp added to it.
    void new_Chart() { m_charts.emplace_back(); };

    /// Wrapper for Adding a BarItem to the last BarChart in m_charts (also changes the Chart timestamp on the background).
    /*!
     * @param timestamp The timestamp of the data.
     * @param label The Bar's Label
     * @param value The Value associated with the Bar.
     * @param category The category in which this bar lies on.
     * @return None. It only builds a BarItem object in the last chart in m_charts.
     */
    void add_BarItem(string &timestamp, string &label, value_t &value, string &category);

    /// Sorts all charts in decreasing order of value, if they are same order lexicographically.
    void sort_all();

    /*!
     * Shows the Graph(Frame) at the selected index with desired amount of bars by the user.
     * @param index Current frame index.
     * @param max_bars Requested number of bars per chart. 
     */ 
    void show(size_t index, size_t max_bars);

    /*!
     * Displays the x-axis of the chart.
     * @param curr_chart Current chart.
     * @param num_bars Number of bars in the chart.
    */
    void show_x_axis(BarChart &curr_chart,size_t num_bars);

    /// Reports a category overflow (this shouldn't be needed but multithreading and race conditions are a thing).
    void report_overflow() {category_overflow = true;};

  };
};

#endif