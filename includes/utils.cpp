#include "utils.h"
#include <cstring>
#include <iostream>
#include <signal.h>
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

void utils::parse_pipes(std::string s, std::vector<command_t> &c) {
  command_t caux;
  int cont = 0, flag = 0;
  std::string aux;
  for (int i = 0; i < s.length(); ++i) {
    if (s[i] == ' ') {
      if (flag == 0) {
        caux.cmd = aux;
        aux.clear();
        flag++;
      } else {
        caux.args.push_back(aux);
        aux.clear();
      }
    } else if (s[i] == '|') {
      i++;
      flag--;
      c.push_back(caux);
      caux.cmd.clear();
      caux.args.clear();
    } else if (i == s.length() - 1) {
      aux += s[i];
      if (flag == 0) {
        caux.cmd = aux;
        aux.clear();
        flag++;
      } else {
        caux.args.push_back(aux);
        aux.clear();
      }
      c.push_back(caux);
      caux.cmd.clear();
      caux.args.clear();
    } else
      aux += s[i];
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
  pid_t pid = fork();
  if (pid < 0) {
    std::cout << "Error\n";

  } else if (pid == 0) {
    pid_t ppid = getppid();
    if (!strcmp(cmd[0], "dance"))
      kill(ppid, SIGUSR1);
    if (!strcmp(cmd[0], "dance"))
      kill(ppid, SIGUSR2);
    if (strcmp(cmd[0], "dance"))
      execvp(cmd[0], cmd.data());
    if (strcmp(cmd[0], "dance"))
      std::cout << "Error al ejecutar comando\n";
    exit(EXIT_FAILURE);
  } else {
    wait(NULL);
  }
}

void utils::execute_p(std::string input) {
  std::vector<command_t> vc;
  utils::parse_pipes(input, vc);
  int npipes = 0;
  for (int i = 0; i < input.size(); ++i)
    if (input[i] == '|')
      npipes++;
  int pipes[npipes][2];
  for (int i = 0; i < npipes; ++i)
    pipe(pipes[i]);
  int cont = 0;
  /* FP */
  if (fork() == 0) {
    dup2(pipes[0][1], 1);
    close(pipes[0][0]);
    int aux = 1;
    while (aux < npipes) {
      close(pipes[aux][0]);
      close(pipes[aux][1]);
      aux++;
    }
    std::vector<char *> cmd = utils::tocstr(vc[0]);
    execvp(cmd[0], cmd.data());
    std::cout << "error1\n";
  }
  if (npipes > 1) {
    while (cont < npipes - 1) {
      if (fork() == 0) {
        dup2(pipes[cont][0], 0);
        dup2(pipes[cont + 1][1], 1);
        for (int i = 0; i < npipes; ++i) {
          if (i == cont)
            close(pipes[i][1]);
          else if (i == cont + 1)
            close(pipes[i][0]);
          else {
            close(pipes[i][0]);
            close(pipes[i][1]);
          }
        }
        std::vector<char *> cmd = utils::tocstr(vc[cont + 1]);
        execvp(cmd[0], cmd.data());
        printf("error2\n");
      }
      cont++;
    }
  }
  if (fork() == 0) {
    dup2(pipes[cont][0], 0);
    close(pipes[cont][1]);
    int aux = 0;
    while (aux < npipes) {
      if (aux == cont) {
        aux++;
        continue;
      }
      close(pipes[aux][0]);
      close(pipes[aux][1]);
      aux++;
    }
    std::vector<char *> cmd = utils::tocstr(vc[cont + 1]);
    execvp(cmd[0], cmd.data());
    printf("error3\n");
  }
  for (int i = 0; i < npipes; ++i) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }
  for (int i = 0; i < npipes + 1; ++i)
    wait(NULL);
}
