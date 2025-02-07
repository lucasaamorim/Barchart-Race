#include "file_parser.h"

/// Helper function to parse an unsigned integer
uint readUnsigned(string &num, Logger::SourceContext &source_context) {
  try {
    return std::stoul(num);
  } catch (std::invalid_argument &e) {
    Logger::logError2("Invalid Argument while Parsing Unsigned Integer",source_context);
  } catch (std::out_of_range &e) {
    Logger::logError2("Out of Range while Parsing Unsigned Integer",source_context);
  }
  return 0;
}

/// Helper function to parse a signed integer
int readSigned(string &num, Logger::SourceContext &source_context) {
  try {
    return std::stoi(num);
  } catch (std::invalid_argument &e) {
    Logger::logError2("Invalid Argument while Parsing Signed Integer",source_context);
  } catch (std::out_of_range &e) {
    Logger::logError2("Out of Range while Parsing Signed Integer",source_context);
  }
  return 0;
}

/// Parses the file and outputs the metadata.
Metadata FileParser::loadFile() {
  std::ifstream file(file_path);

  if (!file.is_open()) {
    Logger::logError1("Could not open file.");
  }

  Frame ref_frame; // Reference Frame that holds only metadata
  source_context.file = file_path;

  // Reading Header of the file 
  string title, x_label, source;

  auto getMeta = [&] () -> string {
    string meta;
    while(file and meta.empty()) {
      getline(file, meta);
      if (meta.empty()) {
        Logger::logWarning2("Ignoring Empty Line while Looking for the Chart's Metadata",source_context);
      }
    }
    return meta;
  };

  title = getMeta();
  x_label = getMeta();
  source = getMeta();
  //Wish I could just do this instead
  //for (auto &meta : {title, x_label, source}) meta = getMeta();

  ref_frame.setMeta(title, x_label, source);

  //Reading data for each Frame
  string line;
  buffer_t tokens;
  while(getline(file, line)) {
    if (line.empty()) continue;
    
    auto tokens = tokenizeLine(line);
    if (tokens.size() == 1) {
      uint n_bars = readUnsigned(tokens.front(), source_context);
      std::unique_ptr<Frame> frame = std::make_unique<Frame>(ref_frame);
      readFrame(file, *frame, n_bars);
      animation_manager->addFrame(std::move(frame));
    } else {
      Logger::logError2("Expected only one Token: the Number of Charts in the new Frame", source_context);
    }
  }

  //Return the metadata
  return {title, x_label, source};
}

/// @brief Reads a certain number of bars from the file and fills a Frame with them.
/// @param file the file input stream to read from 
/// @param frame the frame to fill with the bars
/// @param n_bars the number of bars to read
void FileParser::readFrame(std::ifstream &file, Frame& frame,int n_bars) {
  string line;
  while(n_bars) {
    getline(file, line);
    auto tokens = tokenizeLine(line);
    bool empty = false;

    if (tokens.size() >= 5) {
      std::unique_ptr<Bar> bar = std::make_unique<Bar>();
      string timestamp = readBar(*bar, tokens);
      frame.setTimestamp(timestamp);
      frame.addBar(std::move(bar));
      n_bars--;
    } else if (tokens.size() == 1) {
      Logger::logWarning2("Only one token on the Line, Assuming Premature end of the Chart",source_context);
      return;
    } else {
      Logger::logWarning2("Ignoring Line with Unexpected Number of Tokens",source_context);
    }
  }
}

string FileParser::readBar(Bar &bar, buffer_t &tokens) {
  string timestamp = tokens.front();
  tokens.pop();
  bar.setLabel(tokens.front());
  tokens.pop();
  //Ignore the info in between the label and the value
  while(tokens.size() > 2) tokens.pop();
  bar.setValue(readSigned(tokens.front(), source_context));
  tokens.pop();
  bar.setCategory(tokens.front());
  animation_manager->addCategoryColor(bar.getCategory());
  tokens.pop();

  return timestamp;
}

//FIXME: Treat quotes inside quotes. For now it only assumes that quotes are 
/// Takes a string and returns a queue of tokens.
FileParser::buffer_t FileParser::tokenizeLine(string &line) {
  buffer_t tokens;
  string token;
  bool in_quotes = false; // Flag to check if we are inside a quoted string.
  for (char &c : line) {
    switch(c) {
      case '"':
        in_quotes = !in_quotes;
        break;
      case ',':
        if (not in_quotes) {
          tokens.push(token);
          token.clear();
        } else {
          token += c;
        }
        break;
      default:
        token += c;
        break;
    }
  }

  if (not token.empty()) {
    tokens.push(token);
  }

  return tokens;
}