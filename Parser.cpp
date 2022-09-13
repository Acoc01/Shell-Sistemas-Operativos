#include <bits/stdc++.h>
using namespace std;

vector<string> v;
string com;

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

int main(){
    string in;
    stringstream stream;
    getline(cin, in);

    parse(in);
    cout<<"Comando: "<<com<<endl;
    for(auto i: v){
        cout<<"Argumentos: "<< i<<endl;
    }
    return 0;
}