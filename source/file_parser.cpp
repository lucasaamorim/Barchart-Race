#include "file_parser.h"

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
  std::queue<string> buffer;
  Frame ref_frame;
  if(!file.is_open()){
    Logger::logError1("Erro ao abrir o arquivo.");
  }
  source_context.file = file_path;
  readHeader(file);
  fillFrameMetadata(ref_frame);
  string line;
  while(getLine(file, line, source_context.line)) {
    auto frame_ptr = std::make_unique<Frame>(ref_frame);
    size_t n_tokens = tokenizeLine(line, buffer);
    // Checks if there are too many values ​​given in the bar quantity line.
    if (n_tokens > 1) {
      Logger::logWarning2("The number of bars that will be on the current chart could not be read because too many values were provided in the line.", source_context);
      std::queue<string>().swap(buffer); // Clear the buffer.
      continue;
    }
    buffer.pop();
    int n_bars = validateNumbersBarsForFrame(line);
    if (frame_ptr == nullptr) Logger::logError1("Frame not initialized.");
    processData(n_bars, file, buffer, *frame_ptr);
    animation_manager->addFrame(std::move(frame_ptr));
  }

  for (const auto &category : categories) {
    animation_manager->addCategoryColor(category);
  }
}

/**
 * @brief Reads the header of the file to extract metadata for the chart.
 * 
 * This method reads the first three lines of the file and stores their content 
 * in the corresponding member variables: `main_title`, `x_label`, and `source`.
 * 
 * @param file Reference to the input file stream from which the header 
 * lines will be read.
 */
void FileParser::readHeader(std::ifstream& file){
  getLine(file, metadata.main_title, source_context.line);
  getLine(file, metadata.x_label, source_context.line);
  getLine(file, metadata.source, source_context.line);
}

/**
 * @brief Populates a Frame object with metadata information
 * 
 * This function transfers metadata from the FileParser's metadata structure
 * to the provided Frame object, setting its title, x-axis label, and source.
 * 
 * @param frame Reference to the Frame object to be populated with metadata
 */
void FileParser::fillFrameMetadata(Frame& frame){
  frame.setTitle(metadata.main_title);
  frame.setXLabel(metadata.x_label);
  frame.setSource(metadata.source);
}

/**
 * @brief Validates and converts a string to the number of bars to be displayed in a frame
 * 
 * @param line A string containing the number to be validated
 * @throws std::invalid_argument if the string cannot be converted to a number
 * @throws std::out_of_range if the number is too large for an integer
 * @return int The validated number of bars
 * 
 * @details This function performs the following validations:
 *          - Checks if the string can be converted to a number
 *          - Verifies if the number fits within integer bounds
 *          - Ensures the number is greater than zero
 * 
 * The function logs appropriate error messages if any validation fails.
 */
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

/**
 * @brief Process data from a file to create a frame with bars
 * 
 * This function reads data line by line from a file, tokenizes each line,
 * validates the data, and constructs bars which are added to a frame.
 * The process continues until the specified number of bars is reached or
 * an unrecoverable error occurs.
 * 
 * @param n_bars Number of bars to process
 * @param file Input file stream to read data from
 * @param buffer Queue to store tokenized data temporarily
 * @param frame Frame object to store the processed bars
 * 
 * @note The function handles disrupted reads and validates bar items before processing
 * @note Each valid line should contain a timestamp followed by bar data
 * 
 * @details The function processes each line by:
 *          1. Reading a line from the file
 *          2. Tokenizing the line into the buffer
 *          3. Validating the number of items
 *          4. Creating a bar with the tokenized data
 *          5. Adding the valid bar to the frame
 */
void FileParser::processData(int n_bars, std::ifstream& file, std::queue<string>& buffer, Frame& frame){
  bool disrupted = false; // If true initialize alternative read.
  string line;
  int n_bar_itens;
  for (int i = 0; i < n_bars; i++){
    // Get a line and Tokenize it.
    if (!disrupted) {
      getLine(file, line, source_context.line);
      n_bar_itens = tokenizeLine(line, buffer);
    } else disrupted = false; // Back to normal routine.

    if (!validateNumberBarItens(n_bar_itens, disrupted, buffer)) {
      if (disrupted) {
          break; // Premature end.
      }
      continue; // Ignoring this bar
    }

    std::unique_ptr<Bar> bar = std::make_unique<Bar>();
    frame.setTimestamp(buffer.front());
    buffer.pop();
    if(setBarItens(buffer, *bar)){
      frame.addBar(std::move(bar));
    }
  }
}

/*!
 * @brief Validates the number of items in a bar entry from the input file.
 *
 * This function checks if the number of tokens (items) in a line meets the requirements
 * for creating a valid bar entry. A valid bar entry must have exactly 5 tokens.
 *
 * @param n_itens Reference to the number of items found in the line
 * @param disrupted Reference to a flag indicating if parsing should be discontinued
 * @param buffer Reference to a queue containing the tokens from the line
 *
 * @return bool Returns true if the number of items is valid (5) or too many (>5),
 *              returns false if the number of items is insufficient (<5) or indicates end of chart (1)
 *
 * @warning If more than 5 tokens are found, only the first 5 will be used
 * @warning If exactly 1 token is found, assumes premature end of barchart
 * @warning If less than 5 tokens are found, the line is ignored and buffer is cleared
 */
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

/**
 * @brief Sets the properties of a Bar object from a queue of strings
 * 
 * This function processes a queue of strings to populate a Bar object with its label,
 * value, and category. It follows a specific order of processing: label first,
 * then value, and finally category. The category is also added to the class's
 * categories set.
 * 
 * @param buffer A queue of strings containing the bar's properties in order (label, value, category)
 * @param bar The Bar object to be populated with the properties
 * @return bool Returns true if all properties were set successfully,
 *              false if there was an error validating the bar value
 * 
 * @note The function assumes the buffer contains the correct number of items
 *       in the correct order (label, value, category)
 */
bool FileParser::setBarItens(std::queue<string>& buffer, Bar& bar) {
  int item_type = item_type_bar_e::BAR_LABEL;
  while (!buffer.empty()){
    string item = buffer.front();
    buffer.pop();
    switch (item_type){
      case item_type_bar_e::BAR_LABEL:
        bar.setLabel(item);
        break;
      case item_type_bar_e::VALUE:
        int value;
        if(!validateBarValue(item, value)) return false;
        bar.setValue(value);
        break;
      case item_type_bar_e::CATEGORY:
        bar.setCategory(item);
        categories.insert(item);
        break;
      default:
        break;
    }
    item_type++;
  }
  return true;
}

/**
 * @brief Validates and converts a string to an integer bar value
 * 
 * @param item String containing the value to be validated and converted
 * @param value Reference to store the converted integer value
 * 
 * @return true If the string was successfully converted to an integer
 * @return false If the string is not a valid number or is out of range
 * 
 * @details This function attempts to convert a string to an integer using stoi().
 * It handles invalid input through exception handling for:
 * - std::invalid_argument: When the string is not a valid number
 * - std::out_of_range: When the number is too large for an integer
 * 
 * Both error cases are logged through the Logger system with appropriate messages.
 */
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

/**
 * @brief Retrieves the metadata information from the FileParser object
 * @param main_title Reference to store the main title of the chart
 * @param x_label Reference to store the x-axis label
 * @param source Reference to store the data source information
 */
void FileParser::getMetadata(string& main_title, string& x_label, string& source){
  main_title = metadata.main_title;
  x_label = metadata.x_label;
  source = metadata.source;
}

/*!
  * @param stream Stream to retrieve data from.
  * @param line Reference to string that will recieve the line.
  * @param line_cnt An integer to be incremented for every empty line it jumps over.
  * @return The input stream. It may have the failbit flag set on if it's empty.
  */
std::istream& FileParser::getLine(std::istream &stream, string &line, int &line_cnt) {
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
   * Tokenizes values in a line.
   * @param line Line to tokenize.
   * @param buffer Buffer to push the tokens to.
  */
size_t FileParser::tokenizeLine(string &line, std::queue<string> &buffer) {
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