#ifndef BARCHART_H
#define BARCHART_H

/*!
 * Bar Char Race project.
 *
 * @author Selan
 * @data October 5th, 2020.
 */

#include <string>

using std::string;

#include <chrono>
#include <thread>

#include <utility>
#include <algorithm>
#include <map>

#include <vector>

using std::vector;

#include <iostream>

using std::cout;

#include "libs/text_color.h"
#include "bcr_am.h"


namespace bcra {
// The value type of data item.
  using value_t = long;

/// This class represents a single Bar Chart.
  class BarChart {
  public:
    /// Represents a single bar information.
    struct BarItem {
      string label;    //!< Bar label.
      value_t value;   //!< Bar value.
      string category; //!< Bar category.

      /// Default ctro.
      BarItem(string &l, value_t &v, string &c)
          : label{std::move(l)}, value{v}, category{std::move(c)} { /*empty*/
      }

      // Comparison operators (Used for sorting).
      inline bool operator<(const BarItem &B) const {
        if (this->value != B.value) return this->value < B.value;
        // Lexicographically order the bars if the value is the same
        // Do it reversed so that it will be ordered lexicographically when using greater<>().
        return this->category > B.category;
      }

      inline bool operator>(const BarItem &B) const { return B < *this; }

      inline bool operator<=(const BarItem &B) const { return !(*this > B); }

      inline bool operator>=(const BarItem &B) const { return !(*this < B); }
    };

  private:
    //=== Definition
    string m_time_stamp; //!< Timestamp for the Given Data.
    vector<BarItem> m_bars; //!< Container for the Bars that may be used in the Chart.

  public:
    //== Public interface
    /// Default ctro.
    BarChart() = default;
    /// Default dtro.
    ~BarChart() = default;

    /*!
     * Adds a BarItem to m_bars.
     * @param label Bar label.
     * @param value Bar value.
     * @param category Bar category.
     */ 
    void add_BarItem(string &label, value_t &value, string &category);

    /*!
     * Changes the m_time_stamp.
     * @param timestamp The timestamp of the data.
    */
    void change_timestamp(const string &timestamp) { m_time_stamp = timestamp; }

    /// Wrapper to sort the Bars in Descending order.
    void sort() { std::sort(m_bars.begin(), m_bars.end(), std::greater<>()); }

    /// Returns the timestamp.
    [[nodiscard]]string get_timestamp() const{return m_time_stamp;}

    /// Returns the number of available bars.
    [[nodiscard]]size_t get_n_bars() const{return m_bars.size();}

    /// Display requested numbers of bars, color_coding enabled by overflow flag.
    /*!
     * @param num_bars Number of bars to be displayed.
     * @param color_category Mapping of categories and colors to use it overflow doesn't happen.
     * @param overflow Flag that indicates if the Graph will be colorful or not.
     */
    void display_bars(const size_t &num_bars, std::map<string, short> &color_category, const bool &overflow);

    /*!
     * Returns the value of a bar on the chart.
     * @param idx_bar Bar index.
    */
    value_t get_value_m_BarItem(int idx_bar){return m_bars[idx_bar].value;}

  };

} // namespace bcra.
#endif
