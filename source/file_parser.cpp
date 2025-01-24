#include "file_parser.h"
#include <queue>

string main_title, value_label, source;

void FileParser::loadFile(){
  std::ifstream file(file_path);
  Frame frame;
  if(!file.is_open()){
    error_msg = "Erro ao abrir o arquivo.";
  }
  source_context.file = file_path;
  readHeader(file);
  fillingHeaderFrame(Frame& frame);
  
}

void FileParser::readHeader(std::ifstream& file){
  get_line(file, main_title, source_context.line);
  get_line(file, value_label, source_context.line);
  get_line(file,source, source_context.line);
}

void FileParser::fillingHeaderFrame(Frame& frame){
  // Preenche frame com o header
}

void FileParser::parseData(){

}

bool FileParser::validateFile() const {
  
}

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