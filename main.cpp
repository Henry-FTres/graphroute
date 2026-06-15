#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

vector<string> split(const string& line, char sep){

    vector<string> parts;
    stringstream stream(line); //como um buffer que pode ler pedaço por pedaço
    
    string aux;
    while (getline(stream, aux, sep)){ //lê do stream até encontrar uma vírgula, guarda em aux, e adiciona no vetor. Ele repete até acabar a linha.
        parts.push_back(aux);
    }
    return parts;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Quantidade incorreta de argumentos! Esperado: 2" << "\n";
        return 1;
    }

    ifstream arq(argv[1]);

    if(!arq.is_open()) {
        cerr << "Erro ao abrir arquivo. Passe um caminho válido!" << "\n";
        return 1;
    }

    string auxiliar;
    getline(arq,auxiliar);
    while(getline(arq, auxiliar)){
        auto fields = split(auxiliar, ',');
    }








    arq.close();

    return 0;

}