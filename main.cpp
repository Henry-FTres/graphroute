#include <iostream>
#include <fstream>

using namespace std;

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










    arq.close();

    return 0;

}