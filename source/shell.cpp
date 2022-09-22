#include "utils.h"
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
      command_t com;
      utils::parse(commands, com);
      // for (int i = 0; i < com.args.size(); ++i) {
      //   std::cout << com.args[i] << " ";
      // }
      // std::cout << "\n";
      if (!utils::count_pipes(commands)) {
        utils::execute_cm(com);
      }else{
      	utils::pipes(commands,com);
      }
    } else {
      // If getline doesn't work
      std::cout << "[-]Error al leer comando, saliendo..." << std::endl;
      exit(0);
    }
  }
  return 0;
}
