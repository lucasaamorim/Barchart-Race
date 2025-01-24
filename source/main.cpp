#include <cstdlib> // EXIT_SUCCESS
#include <vector>
#include <iostream>

#include "animation.h"
#include "file_parser.h"

void printUsage();
void printWelcome();
void parseArgs(int argc, char **argv);


int main(int argc, char **argv) {

  //TODO: Fazer Parsing da linha de comando para pegar o arquivo.
  //TODO: Adaptar a rotina para ler um arquivo e só depois rodar a animação.
  parseArgs(argc, argv);

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
}

void printWelcome() {
  std::cout << std::setw(50) << std::setfill('=') << std::endl;
  std::cout << "  Welcome to the Bar Chart Race, v2.0\n";
  std::cout << std::setw(50) << std::setfill('=') << std::endl;
}

void parseArgs(int argc, char **argv) {
  if (argc < 2) {
    printUsage();
    return;
  } else {
    
  }
}