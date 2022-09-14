#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

const int READ = 0;
const int WRITE = 1;

int main(void) {
  std::vector<std::string> colour{"BLUE", "RED"};
  std::vector<std::string> c1{"ps", "auxf"};
  std::vector<std::string> c2{"head", "-5"};
  std::vector<std::string> c3{"rev"};

  int pipe1[2];
  int pipe2[2];
  std::vector<std::vector<char *>> args;
  std::vector<char *> aux;
  for (int i = 0; i < c1.size(); ++i) {
    aux.push_back(strdup(c1[i].c_str()));
  }
  aux.push_back(nullptr);
  args.push_back(aux);
  aux.clear();
  for (int i = 0; i < c2.size(); ++i) {
    aux.push_back(strdup(c2[i].c_str()));
  }
  aux.push_back(nullptr);
  args.push_back(aux);
  aux.clear();
  for (int i = 0; i < c3.size(); ++i) {
    aux.push_back(strdup(c3[i].c_str()));
  }
  aux.push_back(nullptr);
  args.push_back(aux);
  aux.clear();

  pipe(pipe1);
  pipe(pipe2);

  if (fork() == 0) {
    dup2(pipe1[WRITE], WRITE);
    close(pipe1[READ]);
    close(pipe2[READ]);
    close(pipe2[WRITE]);

    execvp(args[0][0], args[0].data());
    printf("error1");
  }
  if (fork() == 0) {
    dup2(pipe1[READ], READ);
    dup2(pipe2[WRITE], WRITE);
    close(pipe1[WRITE]);
    close(pipe2[READ]);

    execvp(args[1][0], args[1].data());
    printf("error2");
  }
  if (fork() == 0) {
    dup2(pipe2[READ], READ);
    close(pipe2[WRITE]);
    close(pipe1[READ]);
    close(pipe1[WRITE]);

    execvp(args[2][0], args[2].data());
    printf("error3");
  }

  close(pipe1[READ]);
  close(pipe1[WRITE]);
  close(pipe2[READ]);
  close(pipe2[WRITE]);

  for (int i = 0; i < 3; ++i)
    wait(NULL);
  printf("\nTermino\n");
  return 0;
}
