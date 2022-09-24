#include <bits/stdc++.h>
using namespace std;

vector<string> v;
string com;
struct command_t {
  int narg;                      // Number of args
  std::vector<std::string> args; // Argument list
  std::string cmd;
};
void parse(string s){
    int flag=0;
    string aux;
    for(int i=0; i<s.length();i++){
        aux+=s[i];
        if(s[i]==' '||i==s.length()-1){
            if(flag==0){
                com=aux;
                aux.clear();
                flag++;
            }else{
                v.push_back(aux);
                aux.clear();
            }
        }

    }
}
vector<command_t> piparse(string s){
    vector<command_t> v;
    command_t caux;
    int cont=0, flag =0;
    string aux;
    for(int i=0; i<s.length();i++){
        if(s[i]==' '){
            if(flag==0){
                caux.cmd=aux;
                aux.clear();
                flag++;
            }else{
                caux.args.push_back(aux);
                aux.clear();
            }
        }else if(s[i]=='|'){
            i++;
            flag--;
            v.push_back(caux);
            caux.cmd.clear();
            caux.args.clear();
        }else if(i==s.length()-1){
            aux+=s[i];
            if(flag==0){
                caux.cmd=aux;
                aux.clear();
                flag++;
            }else{
                caux.args.push_back(aux);
                aux.clear();
            }
            v.push_back(caux);
            caux.cmd.clear();
            caux.args.clear(); 
        }else aux+=s[i];
    }
    return v;
}


int main(){
    string in;
    stringstream stream;
    getline(cin, in);

    vector<command_t> v = piparse(in);
    int cont =0;
    for(auto i: v){
        cout<<"Comando numero "<<cont<<": "<<i.cmd<<endl;
        cout<<"Argumentos: "<<endl;
        for( auto j: i.args){
            cout<<j<<endl;
        }
        cont++;
    }
    return 0;
}