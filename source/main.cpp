#include <cstdlib> // EXIT_SUCCESS
#include <vector>
#include <iostream>

#include "animation.h"

void print_usage();

int main(int argc, char **argv) {
  AnimationManager animation;

  //TODO: Fazer Parsing da linha de comando para pegar o arquivo.
  //TODO: Adaptar a rotina para ler um arquivo e só depois rodar a animação.
  if (argc < 2) print_usage();
  else {

  }

  return EXIT_SUCCESS;
}


void print_usage() {

}