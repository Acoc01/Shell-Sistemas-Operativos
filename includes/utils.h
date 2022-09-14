#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

// Struct of a command
struct command_t {
  int narg;                      // Number of args
  std::vector<std::string> args; // Argument list
  std::string cmd;
};

namespace utils {
void parse(std::string in, command_t &c);                     // Without pipes
void parse_pipes(std::string in, std::vector<command_t> &cs); // With pipes
void execute_cm(command_t c);                                 // Without pipes
void execute_cm(command_t, uint32_t np); // When using pipes
};                                       // namespace utils

#endif
