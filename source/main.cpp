#include <cstdlib> // EXIT_SUCCESS
#include <vector>
#include <iostream>
#include <memory>

#include "animation.h"
#include "file_parser.h"

void printUsage();
void printWelcome();
void parseArgs(int argc, char **argv);

int fps = 24;
int bars = 5;
string filepath = "";

int main(int argc, char **argv) {

  parseArgs(argc, argv);
  printWelcome();

  std::shared_ptr<AnimationManager> animation = std::make_shared<AnimationManager>();
  FileParser parser(filepath, animation);
  readInput(parser, animation);
  
  string enter_to_entry;
  getline(std::cin, enter_to_entry);
  animation->PlayAnimation(fps,bars);

  return EXIT_SUCCESS;
}


void printUsage() {
  std::cout << "Usage: bcr [<options>] <input_data_file>\n";
  std::cout << "Bar Chart Race options:\n";
  std::cout << "\t-b <num> Max # of bars in a single char.\n";
  std::cout << "\t\tValid range is [1,15]. Default value is 5.\n";
  std::cout << "\t-f <num> Animation speed in fps (frames per second).\n";
  std::cout << "\t\tValid range is [1,24]; Default value is 24.";
  std::cout << std::endl;
  exit(1);
}

void printWelcome() {
  std::cout << "==============================================\n";
  std::cout << "  Welcome to the Bar Chart Race, v2.0\n";
  std::cout << "==============================================\n";
}

void readInput(FileParser& parser, std::shared_ptr<AnimationManager> animation){
  string title, x_axis_label, source;
  parser.getMetadata(title, x_axis_label, source);
  cout << ">>> Preparing to read input file \"" << filepath << "\"...\n\n";
  cout << ">>> Processing data, please wait.\n";
  parser.loadFile();
  cout << ">>> Input file successfully read.\n";
  cout << ">>> We have " << animation->numberCharts() << " charts, each with " << bars << " bars.\n\n";
  cout << ">>> Animation Speed is " << fps << " fps.\n";
  cout << ">>> Title: " << title << '\n';
  cout << ">>> X axis label: " << x_axis_label << '\n';
  cout << ">>> Source: " << source << '\n';
  cout << ">>> # of categories found: " << animation->numberCategories() << '\n';
  cout << "Press enter to begin the animation.\n";
}

void parseArgs(int argc, char **argv) {
  if (argc < 2) {
    printUsage();
    return;
  } else {
    for (int arg_n = 1; arg_n < argc; arg_n++) {
      if (argv[arg_n][0] == '-') {
        switch (argv[arg_n][1]) {
          case 'b':
            try {
              bars = std::stoi(argv[arg_n+1]);
              if (bars < 1 || bars > 15) {
                throw std::out_of_range("Out of range");
              }
            } catch (std::invalid_argument&) {
              Logger::logWarning1("Invalid argument for bars. Using default value of 5 bars.\n");
            } catch (std::out_of_range&) {
              Logger::logWarning1("Argument for bars is out of range. Using default value of 5 bars.\n");
            }
            break;
          case 'f':
            try {
              fps = std::stoi(argv[arg_n+1]);
              if (fps < 1 || fps > 24) {
                throw std::out_of_range("Out of range");
              }
            } catch (std::invalid_argument&) {
              Logger::logWarning1("Invalid argument for fps. Using default value of 24 fps.\n");
            } catch (std::out_of_range&) {
              Logger::logWarning1("Argument for fps is out of range. Using default value of 24 fps.\n");
            } 
            break;
          default:
            printUsage();
          return;
        }
        arg_n++;
      } else filepath = argv[arg_n];

    }  
  }
}