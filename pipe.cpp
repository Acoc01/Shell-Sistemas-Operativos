#include <bits/stdc++.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

struct command_t {
  vector<string> args;
  string cmd;
};

void parse(string s, vector<command_t> &c) {
  command_t caux;
  int cont = 0, flag = 0;
  string aux;
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

std::vector<char *> tocstr(command_t c) {
  std::vector<char *> aux;
  aux.push_back(strdup(c.cmd.c_str()));
  size_t len = c.args.size();
  for (size_t i = 0; i < len; ++i) {
    aux.push_back(strdup(c.args[i].c_str()));
  }
  aux.push_back(nullptr);
  return aux;
}

int main() {
  string input;
  getline(cin, input);
  vector<command_t> vc;
  cout << "Parseando...\n";
  parse(input, vc);
  cout << "Parseado\n";
  /*Pipes*/
  cout << "Creando pipes\n";
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
    vector<char *> cmd = tocstr(vc[0]);
    execvp(cmd[0], cmd.data());
    cout << "error1\n";
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
        vector<char *> cmd = tocstr(vc[cont + 1]);
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
    vector<char *> cmd = tocstr(vc[cont + 1]);
    execvp(cmd[0], cmd.data());
    printf("error3\n");
  }
  for (int i = 0; i < npipes; ++i) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }
  for (int i = 0; i < npipes + 1; ++i)
    wait(NULL);
  return 0;
}
