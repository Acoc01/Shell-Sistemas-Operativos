#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

// Struct of a command
struct command_t {
  int narg;         // Number of args
  std::string args; // Argument list
};

#endif
