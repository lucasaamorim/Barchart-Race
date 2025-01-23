#include <algorithm>

using std::transform;

#include <string>

using std::string;

#include <sstream>

using std::istringstream;

#include "bcr_am.h"

namespace bcra {
// Instantiation of the global configuration variable.
  const Cfg global_cfg;

  auto str_lowercase = [](const char *t) -> std::string {
    std::string str{t};
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
  };

  AnimationManager::AnimationManager() {
    m_opt.input_filename = "";
    m_opt.fps = bcra::Cfg::default_fps;
    m_opt.n_bars = bcra::Cfg::default_bars;
    m_read_status = read_status_e::NOT_READ;
    m_animation_state = ani_state_e::START;
    m_db = new Database();
    curr_frame = 0;
  }

/// Initializes the animation engine.
  void AnimationManager::initialize(int argc, char **argv) {
    string error_msg;
    for (int i = 1; i < argc; i++) {
      string opt = str_lowercase(argv[i]);
      if (opt_mp.find(opt) != opt_mp.end()) {
        switch (opt_mp[opt]) {
          case opt_e::MAX_BARS: {
            // Keep in mind that this is only an upper bound to the number of bars.
            opt = str_lowercase(argv[++i]);
            auto n_bars{(short) std::stoi(opt)};

/*================================== Exception Handling ==============================================================*/
            if (n_bars > Cfg::max_bars) {
              string max = std::to_string(Cfg::max_bars);
              error_msg = "Number of bars cannot be over "+max+". Using"+max+"  instead.";
              log::Warning1(error_msg);
              n_bars = Cfg::max_bars;
            } else if (n_bars < Cfg::min_bars) {
              string min = std::to_string(Cfg::min_bars);
              error_msg = "Number of bars needs to be at least "+min+". Using "+min+" instead.";
              log::Warning1(error_msg);
              n_bars = Cfg::min_bars;
            }
/*====================================================================================================================*/

            m_opt.n_bars = n_bars;
            break;
          }
          case opt_e::FPS: {
            opt = str_lowercase(argv[++i]);

/*================================== Exception Handling ==============================================================*/
            auto fps{(short) std::stoi(opt)};
            if (fps > Cfg::max_fps) {
              string max = std::to_string(Cfg::max_fps);
              error_msg = "FPS cannot exceed "+max+". Using "+max+" instead.";
              log::Warning1(error_msg);
              fps = Cfg::max_fps;
            } else if (fps < Cfg::min_fps) {
              string min = std::to_string(Cfg::min_fps);
              error_msg = "FPS has to be at least "+min+". Using "+min+" instead";
              log::Warning1(error_msg);
              fps = Cfg::min_fps;
            }
/*====================================================================================================================*/

            m_opt.fps = fps;
            break;
          }
        }
      } else { //Assume it's the data file.
        if (std::filesystem::exists(opt)) { // Check if file exists.
          m_opt.input_filename = opt;
          return; // No more arguments to be parsed.
        } else {
          error_msg = "File " + opt + " doesn't exist.\n";
          log::Error1(error_msg);
        }
      }
    }
    if (argc <= 1) {
      usage();
      exit(0);
    }
    if (m_opt.input_filename.empty()) {
      error_msg = "Please provide a file.";
      log::Error1(error_msg);
    }

  }

  bool AnimationManager::is_over() const {
    return m_animation_state == ani_state_e::END;
  }

  void AnimationManager::process_events() {
    switch (m_animation_state) {
        case ani_state_e::ERROR:
        break;
      case ani_state_e::READING_INPUT:
      // Wait for User confirmation to Start the animation.
        if (m_read_status == read_status_e::OK){
          string trash;
          getline(std::cin, trash);
        }
        break;
      case ani_state_e::RACING:
        break_time(m_opt.fps);
        break;
      default:
        break;
    }
  }

  void AnimationManager::update() {
    switch (m_animation_state) {
      case ani_state_e::START:
        m_animation_state = ani_state_e::WELCOME;
        break;
      case ani_state_e::ERROR:
        m_animation_state = ani_state_e::READING_INPUT;
        m_read_status = read_status_e::OK;
        break;
      case ani_state_e::WELCOME:
        m_animation_state = ani_state_e::READING_INPUT;
        break;
      case ani_state_e::READING_INPUT:
        if (m_read_status == read_status_e::NOT_READ) {
          m_read_status = read_file();
        } else if (m_read_status == read_status_e::OK) {
          m_animation_state = ani_state_e::RACING;
        } else { // Status is "ERROR"
          m_animation_state = ani_state_e::ERROR;
        }
        break;
      case ani_state_e::RACING:
        if (curr_frame >= m_db->get_n_charts()) {// Already got to the last frame.
          m_animation_state = ani_state_e::END;
        }
        break;
      default:
        break;
    }
  }

  void AnimationManager::render() {
    // TODO
    switch (m_animation_state) {
      case ani_state_e::ERROR:
      {
          error_e error_type;
          string error_message;
          log::SourceContext sc;
        for(auto &i : m_error_msgs){
          tie(error_type,error_message,sc) = i;
          switch (error_type)
          {
          case error_e::WARNING1:
            log::Warning1(error_message);
            break;
          case error_e::WARNING2:
            log::Warning2(error_message, sc);
            break;
          case error_e::ERROR1:
            log::Error1(error_message);
            break;
          case error_e::ERROR2:
            log::Error2(error_message,sc);
            break;
          default:
            log::Message(error_message);
            break;
          }
        }
        m_error_msgs.clear(); // No need to keep storing already displayed messages.
      }
        break;
      case ani_state_e::WELCOME:
        std::cout << "==============================================\n";
        std::cout << "  Welcome to the Bar Chart Race, v1.0\n";
        std::cout << "==============================================\n";
        break;
      case ani_state_e::READING_INPUT:
        if (m_read_status == read_status_e::NOT_READ) {
          std::cout << ">>> Preparing to read input file \"" << m_opt.input_filename << "\"...\n\n";
          std::cout << ">>> Processing data, please wait.\n";
        } else if (m_read_status == read_status_e::OK) {
          std::cout << ">>> Input file successfully read.\n";
          std::cout << ">>> We have " << m_db->get_n_charts() << " charts, each with " << m_opt.n_bars << " bars.\n\n";
          std::cout << ">>> Animation Speed is " << m_opt.fps << " fps.\n";
          std::cout << ">>> Title: " << m_db->get_title() << '\n';
          std::cout << ">>> Value is: " << m_db->get_value_label() << '\n';
          std::cout << ">>> Source is: " << m_db->get_source() << '\n';
          std::cout << ">>> # of categories found: " << m_db->get_n_categories() << '\n';
          std::cout << "Press enter to begin the animation.\n";
        }
        // If the read status is an error. the update function will take it to the error state.
        break;
      case ani_state_e::RACING:
        m_db->show(curr_frame, m_opt.n_bars);
        curr_frame++;
        break;
      case ani_state_e::END:
        std::cout << "Hope you have enjoyed the Bar Chart Race!\n\n";
      default:
        break;
    }
  }

  void usage() {
    std::cout << "Usage: bcr [<options>] <input_data_file>\n";
    std::cout << "Bar Chart Race options:\n";
    std::cout << "\t-b <num> Max # of bars in a single char.\n";
    std::cout << "\t\tValid range is [1,15]. Default value is 5.\n";
    std::cout << "\t-f <num> Animation speed in fps (frames per second).\n";
    std::cout << "\t\tValid range is [1,24]; Default value is 24.\n";
  }

  AnimationManager::read_status_e AnimationManager::read_file() {
    string error_msg;
    read_status_e read_status = read_status_e::OK; // It's all ok until it isn't.
    bool disrupted = false; // If true initialize alternative read.
    std::queue<string> buffer;
    // Do not run if it has already been run.
    if (m_read_status == read_status_e::OK || m_read_status == read_status_e::ERROR) return m_read_status;
    // Open the file (we already know that exists so no need to check again).
    std::ifstream file(m_opt.input_filename);
    source_context.file = m_opt.input_filename;
    enum val_types_e {
      CHART_TITLE = 0,
      BAR_LABEL,
      USELESS,
      AMOUNT,
      TYPE
    };// P.S.: There's also the main title, value label and source, but they appear only once in the file.
    { // Put it in different scope so that they get destroyed as soon as possible.
      // Get these values that appear once in the file.
      string main_title, value_label, source;
      get_line(file, main_title, source_context.line);
      get_line(file, value_label, source_context.line);
      get_line(file,source, source_context.line);
      
      /// Give the first inputs to the Database.
      m_db->init(main_title, value_label, source);
    }

    value_t curr_color{0};
    // Now get the repeating values.
    string val;
    while(get_line(file, val, source_context.line)) {
      error_msg.clear();
      size_t n_tokens = tokenize_line(val, buffer);

      // Checks if there is too many values given in the line.
      if (n_tokens > 1 && !disrupted) {
        error_msg = "The number of bars that will be on the current chart could not be read because too many values were provided in the line.";
        m_error_msgs.emplace_back(error_e::WARNING2,error_msg,source_context);
        read_status = read_status_e::ERROR;
        std::queue<string>().swap(buffer); // Clear the buffer.
        continue;
      }

      val = buffer.front();
      buffer.pop();
      int n_bars;
/*================================== Exception Handling ==============================================================*/
      try { //These are fatal errors. If they happen, there is no need to continue reading the file.
        n_bars = stoi(val);
      } catch (std::invalid_argument&) {
        error_msg = "The value provided is not a number.";
        m_error_msgs.emplace_back(error_e::ERROR2,error_msg,source_context);
        return read_status_e::ERROR;
      } catch (std::out_of_range&) {
        error_msg = "The given value is too big. ";
        m_error_msgs.emplace_back(error_e::ERROR2,error_msg,source_context);
        return read_status_e::ERROR;
      }
      if (n_bars <= 0) {
        error_msg = "Invalid Number of Bars.";
        m_error_msgs.emplace_back(error_e::ERROR2,error_msg,source_context);
        return read_status_e::ERROR;
      }
/*====================================================================================================================*/

      string timestamp, label, category;
      value_t value;
      // Build new BarChart to add the elements.
      m_db->new_Chart();



      // Read all BarItems.
      for (int i = 0; i < n_bars; i++) {
        int val_type = val_types_e::CHART_TITLE;
        // Get a line and Tokenize it.
        if (!disrupted) {
          get_line(file, val, source_context.line);
          n_tokens = tokenize_line(val, buffer);
        } else disrupted = false; // Back to normal routine.

/*================================== Exception Handling ==============================================================*/
        // Check if there is an incorrect amount of tokens in the line.
        if (n_tokens > 5) { // Warning.
          error_msg = "Too many arguments for a Bar. Using first 5 values in line.";
          m_error_msgs.emplace_back(error_e::WARNING2,error_msg,source_context);
        }
        if (n_tokens == 1) { // Warning.
          error_msg = "Assuming Premature end of Barchart";
          m_error_msgs.emplace_back(error_e::WARNING2,error_msg,source_context);
          read_status = read_status_e::ERROR;
          disrupted = true;
          break;
        }
        if (n_tokens < 5) { // FATAL.
          error_msg = "Incorrect amount of tokens in the line. Ignoring this Bar.";
          m_error_msgs.emplace_back(error_e::WARNING2,error_msg,source_context);
          std::queue<string>().swap(buffer); // Clear the buffer.
          continue;
        }
/*====================================================================================================================*/

        // Run through tokens and assign their respective values.
        while (!buffer.empty()) {
          auto token = buffer.front();
          buffer.pop();
          switch (val_type) {
            case val_types_e::CHART_TITLE:// Timestamp.
              timestamp = token;
              break;
            case val_types_e::BAR_LABEL:// Bar Label.
              label = token;
              break;
            case val_types_e::AMOUNT:// Bar value.
              try { // Using stoi's built-in exceptions and providing coms standard errors.
                value = stoi(token);
              } catch (std::invalid_argument&) {
                error_msg = "Provided Bar value is not a number.";
                m_error_msgs.emplace_back(error_e::WARNING2,error_msg,source_context);
                read_status = read_status_e::ERROR;
              } catch (std::out_of_range&) {
                error_msg = "Provided Bar value is too big.";
                m_error_msgs.emplace_back(error_e::WARNING2,error_msg,source_context);
                read_status = read_status_e::ERROR;
              }
              break;
            case val_types_e::TYPE:// Bar category.
              category = token;
              // Check if a new color needs to be added for this category and if there are enough colors.
              if (curr_color >= Color::color_list.size()) {
                m_db->report_overflow();
              } else if (!m_db->find_category(category)) { // Category not found in color map.
                m_db->add_category(category, Color::color_list[curr_color]);
                curr_color++;
              }
              break;
            default:
              break;
          }
          val_type++;
        }

        // Add BarItem to current Barchart.
        m_db->add_BarItem(timestamp, label, value, category);
      }
    }
    // Sort all charts.
    m_db->sort_all();

    return read_status;
  }

  size_t tokenize_line(string &line, std::queue<string> &buffer) {
    size_t first{0}, last{0}, n_tokens{0};
    string token;
    string delimiters = ",\n";
    // Go through the line.
    while (last < line.size()) {
      //Check if current character is not a delimiter (the token continues).
      if (delimiters.find(line[last]) == string::npos) {
        last++;
      } else { // Reached delimiter (we have the range of a new token to add).
        token = line.substr(first, last - first);
        if (!token.empty()) {
          // Ignore empty tokens
          buffer.push(token);
          n_tokens++;
          // These may appear due to two separators being used one after another.
        }
        last++;
        first = last;
      }
    }
    if (first != last) {// Edge case handling.
      // Push back a token manually if there are no delimiters in the end.
      token = line.substr(first, last - first);
      buffer.push(token);
      n_tokens++;
    }
    return n_tokens;
  }

  void break_time(const int &fps) {
    std::chrono::milliseconds duration{1000 / fps};
    // Halt execution for the specified duration.
    std::this_thread::sleep_for(duration);
  }

  std::istream& get_line(std::istream &stream, string &line, int &line_cnt) {
    line.clear();
    char c;
    while(line.empty()) {
      c = '\0'; // Null terminator as placeholder.
      while(stream.get(c) && c != '\n') line.push_back(c);

      //set failbit flag if in EOF.
      if (c == '\0') {
        // This is necessary for when an empty line is the last one, so it won't get in a perpetual loop (Since the line would be empty).
        stream.setstate(std::_S_eofbit);
        break;
      }
      line_cnt++;
    }
    // Unset failbit if something was extracted during the loop.
    if (!line.empty()) stream.clear();
    return stream;
  }
}; // namespace bcra
