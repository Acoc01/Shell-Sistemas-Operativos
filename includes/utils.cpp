#include "utils.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

void utils::parse(std::string in, command_t &c) {
  int flag = 0, len = in.length();
  std::string aux;
  for (int i = 0; i < len; ++i) {
    if (in[i] != ' ')
      aux += in[i];
    if (in[i] == ' ' || i == len - 1) {
      if (!flag) {
        c.cmd = aux;
        aux.clear();
        flag++;
      } else {
        c.args.push_back(aux);
        aux.clear();
      }
    }
  }
}

/*Transform a std::string to char* */
std::vector<char *> utils::tocstr(command_t c) {
  std::vector<char *> aux;
  aux.push_back(strdup(c.cmd.c_str())); // Always push the cmd first
  size_t len = c.args.size();
  for (size_t i = 0; i < len; ++i) {
    aux.push_back(strdup(c.args[i].c_str())); // Then push his args
  }
  aux.push_back(nullptr);
  return aux;
}
/*Counts how many pipes there are*/
int utils::count_pipes(std::string in) {
  size_t len = in.size();
  int cont = 0;
  for (size_t i = 0; i < len; ++i) {
    if (in[i] == '|')
      cont++;
  }
  return cont;
}

void utils::execute_cm(command_t c) {
  std::vector<char *> cmd = utils::tocstr(c);
  int pid = fork();
  if (pid < 0) {
    std::cout << "Error\n";

  } else if (pid == 0) {
    execvp(cmd[0], cmd.data());
    std::cout << "Error al ejecutar comando\n";
    exit(EXIT_FAILURE);
  } else {
    wait(NULL);
  }
}
