#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

int main() {
  system("clear");
  std::string prompt = "test$ ";
  std::string commands;
  while (1) {
    std::cout << prompt;
    if (std::getline(std::cin, commands)) {
      if (commands == "exit")
        exit(0);
      // Parse commands
    } else {
      // If getline doesn't work
      std::cout << "[-]Error al leer comando, saliendo..." << std::endl;
      exit(0);
    }
  }
  return 0;
}
