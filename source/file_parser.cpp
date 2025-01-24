#include "file_parser.h"

string main_title, x_label, source;

enum item_type_bar_e{
  BAR_LABEL = 0,
  USELESS,
  VALUE,
  CATEGORY
};

/**
 * @brief Loads a file and processes its data to generate animation frames.
 * 
 * This function opens a file specified by the `file_path`, reads the file header, 
 * processes chart data, and creates `Frame` objects that are added to the animation 
 * manager (`animation_manager`).
 */
void FileParser::loadFile(){
  std::ifstream file(file_path);
  std::unique_ptr<Frame> frame = std::make_unique<Frame>();
  std::queue<string> buffer;
  if(!file.is_open()){
    Logger::logError1("Erro ao abrir o arquivo.");
  }
  source_context.file = file_path;
  readHeader(file);
  fillFrameHeader(frame);
  string line;
  while(get_line(file, line, source_context.line)) {
    size_t n_tokens = tokenize_line(line, buffer);
    // Checks if there are too many values ​​given in the bar quantity line.
    if (n_tokens > 1) {
      Logger::logWarning2("The number of bars that will be on the current chart could not be read because too many values were provided in the line.", source_context);
      std::queue<string>().swap(buffer); // Clear the buffer.
      continue;
    }
    buffer.pop();
    int n_bars = validateNumbersBarsForFrame(line);
    frame->setTimestamp(buffer.front());
    buffer.pop();
    processData(n_bars, file, buffer, frame);
    animation_manager->addFrame(std::move(frame));
  }
}

void FileParser::readHeader(std::ifstream& file){
  get_line(file, main_title, source_context.line);
  get_line(file, x_label, source_context.line);
  get_line(file,source, source_context.line);
}

void FileParser::fillFrameHeader(std::unique_ptr<Frame>& frame){
  frame->setTitle(main_title);
  frame->setXLabel(x_label);
  frame->setSource(source);
}

int FileParser::validateNumbersBarsForFrame(string& line){
  int n_bars;
  try { //These are fatal errors. If they happen, there is no need to continue reading the file.
    n_bars = std::stoi(line);
  } catch (std::invalid_argument&) {
    Logger::logError2("The value provided is not a number.", source_context);
  } catch (std::out_of_range&) {
    Logger::logError2("The given value is too big.", source_context);
  }
  if (n_bars <= 0) {
    Logger::logError2("Invalid Number of Bars.", source_context);
  }
  return n_bars;
}

void FileParser::processData(int n_bars, std::ifstream& file, std::queue<string>& buffer, std::unique_ptr<Frame>& frame){
  bool disrupted = false; // If true initialize alternative read.
  string line;
  int n_bar_itens;
  for (int i = 0; i < n_bars; i++){
    // Get a line and Tokenize it.
    if (!disrupted) {
      get_line(file, line, source_context.line);
      n_bar_itens = tokenize_line(line, buffer);
    } else disrupted = false; // Back to normal routine.

    if (!validateNumberBarItens(n_bar_itens, disrupted, buffer)) {
      if (disrupted) {
          break; // Premature end.
      }
      continue; // Ignoring this bar
    }

    std::unique_ptr<Bar> bar = std::make_unique<Bar>();
    if(setBarItens(buffer, bar)){
      frame->addBar(std::move(bar));
    }
  }
}

bool FileParser::validateNumberBarItens(int& n_itens, bool& disrupted, std::queue<string>& buffer){
  // Check if there is an incorrect amount of tokens in the line.
  if (n_itens > 5) { // Warning.
    Logger::logWarning2("Too many arguments for a Bar. Using first 5 values in line.", source_context);
    return true;
  }
  if (n_itens == 1) { 
    Logger::logWarning2("Assuming Premature end of Barchart.", source_context);
    disrupted = true;
    return false;
  }
  if (n_itens < 5) {
    Logger::logWarning2("Incorrect amount of tokens in the line. Ignoring this Bar.", source_context);
    std::queue<string>().swap(buffer); // Clear the buffer.
    return false;
  }
  return true;
}

bool FileParser::setBarItens(std::queue<string>& buffer, std::unique_ptr<Bar>& bar) {
  int item_type = item_type_bar_e::BAR_LABEL;
  while (!buffer.empty()){
    string item = buffer.front();
    buffer.pop();
    switch (item_type){
      case item_type_bar_e::BAR_LABEL:
        bar->setLabel(item);
        break;
      case item_type_bar_e::VALUE:
        int value;
        if(!validateBarValue(item, value)) return false;
        bar->setValue(value);
        break;
      case item_type_bar_e::CATEGORY:
        bar->setCategory(item);
        categories.insert(item);
        break;
      default:
        break;
    }
    item_type++;
  }
  return true;
}

bool FileParser::validateBarValue(string& item, int& value){
  string error_msg;
  try { // Using stoi's built-in exceptions and providing coms standard errors.
    value = stoi(item);
  } catch (std::invalid_argument&) {
    Logger::logWarning2("Provided Bar value is not a number.", source_context);
    return false;
  } catch (std::out_of_range&) {
    Logger::logWarning2("Provided Bar value is too big.", source_context);
    return false;
  }
  return true;
}

/*!
   * Tokenizes values in a line.
   * @param line Line to tokenize.
   * @param buffer Buffer to push the tokens to.
  */
std::istream& FileParser::get_line(std::istream &stream, string &line, int &line_cnt) {
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

/*!
  * @param stream Stream to retrieve data from.
  * @param line Reference to string that will recieve the line.
  * @param line_cnt An integer to be incremented for every empty line it jumps over.
  * @return The input stream. It may have the failbit flag set on if it's empty.
  */
size_t FileParser::tokenize_line(string &line, std::queue<string> &buffer) {
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