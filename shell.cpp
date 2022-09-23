#include "utils.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>
#include <fstream>
#include <sys/resource.h>
#include <sys/wait.h>

int main() {
  system("clear");
  std::string prompt = "test$ ";
  std::string commands;
  std::string file;
  struct rusage usgi;
  struct rusage usgf;
  std::ofstream ofs;
  int usr = 0;
  while (1) {
    std::cout << prompt;
    if (std::getline(std::cin, commands)) {
      if (commands == "exit")
        exit(0);
      // Parse commands
      command_t com;
      utils::parse(commands, com);
      if(com.cmd == "usorecursos"){
        if(!usr){
          if(com.args[0] == "start")
          {
            usr = 1;
            prompt = "test> ";
            file = com.args[1];
            ofs.open(file);
            ofs << "Comando\t\t\t\tUSTIME\t\t\t\tSYSTIME\t\t\t\tMXRS\n";
          }
        }else{
          if(usr)
          {
            usr = 0;
            prompt = "test$ ";
            ofs.close();
          }
        }
      }
      // for (int i = 0; i < com.args.size(); ++i) {
      //   std::cout << com.args[i] << " ";
      // }
      // std::cout << "\n";
      if (!utils::count_pipes(commands) && prompt == "test$ ") {
        if(com.cmd != "usorecursos")utils::execute_cm(com);
      }else if(!utils::count_pipes(commands) && com.cmd != "usorecursos"){
        getrusage(RUSAGE_CHILDREN, &usgi);
        std::vector<char *> cmd = utils::tocstr(com);
        int pid = fork();
        if (pid < 0) {
          std::cout << "Error\n";

        } else if (pid == 0) {
          execvp(cmd[0], cmd.data());
          std::cout << "Error al ejecutar "<<cmd[0]<<"\n";
          exit(EXIT_FAILURE);
        } else {
          wait(NULL);
        }
        getrusage(RUSAGE_SELF, &usgf);
        ofs << commands<<"\t\t\t\t" << usgi.ru_utime.tv_usec <<"ms\t\t\t\t"
        <<usgi.ru_stime.tv_usec << "ms\t\t\t\t" << usgi.ru_maxrss<<
        "KB\n";
      }
    } else {
      // If getline doesn't work
      std::cout << "[-]Error al leer comando, saliendo..." << std::endl;
      exit(0);
    }
  }
  return 0;
}
