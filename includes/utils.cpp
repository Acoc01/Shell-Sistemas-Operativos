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
pair <char*, vector<char*>> utils::clean(vector<char *> c){
    char* command= c[0];
    vector<char*> v1;
    int cont=0;
    for(vector<char*>::iterator i = c.begin(); i!=c.end();i++){
        cont++;
        if(*i=="|"){
            break;
        }else {
            v1.push_back(*i);
        }
    }
    c.erase(c.begin(), c.begin()+cont);
    auto p = make_pair(command, v1);
    return p;
}

void utils::pipes(std::string in, command_t c){
    int status;
    int tam=utils::count_pipes(in);
    int pipes[tam][2];
    int place=0;
    char* com;
    vector<char*> s;
    vector<char*> vec=utils::tocstr(c);
    pair<char*, vector<char*>> dat;

    if(fork()==0){
        dup2(pipes[0][1], 1);
        for(int i=0;i<tam;i++){
            for(int j=0;i<2;j++){
                close(pipes[i][j]);
            }
        }
	
	dat=clean(vec);
        com=dat.first;
        s=dat.second;

        execvp(com,s.data());

        place++;
    }else{
    	if(fork()==0){
            for(place;place<tam;place++){
               dup2(pipes[place-1][0], 0);
               dup2(pipes[place][1], 1);
               for(int i=0;i<tam;i++){
                    for(int j=0;i<2;j++){
                        close(pipes[i][j]);
                    }
                }
                dat=clean(vec);
                com=dat.first;
                s=dat.second;

                execvp(com,s.data());
            }
       }else{
          if(fork()==0){
             dup2(pipes[place][0], 0);
             for(int i=0;i<tam;i++){
                for(int j=0;i<2;j++){
                   close(pipes[i][j]);
                }
             }
             dat=clean(vec);
             com=dat.first;
             s=dat.second;

             execvp(com,s.data());
           }
        }
        for(int i=0;i<tam;i++){
            for(int j=0;i<2;j++){
                close(pipes[i][j]);
            }
        }
        for(int i=0;i<tam;i++){
            for(int j=0;i<2;j++){
                wait(&status);
            }
        }
    }
}
