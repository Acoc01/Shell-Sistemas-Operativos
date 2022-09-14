#include "utils.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

void utils::parse(std::string in, command_t &c) {
  int flag = 0, len = in.length();
  std::string aux;
  for (int i = 0; i < len; ++i) {
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

void utils::execute_cm(command_t c) {}
