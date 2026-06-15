#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "graphroute.cpp"

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

int menu(){
    int opcao;
    while (true) {
        cout << "================================================================\n"
             << "1. Exibir grafo completo.\n"
             << "2. Encontrar menor caminho.\n"
             << "3. Calcular diâmetro do grafo.\n"
             << "4. Identificar Roteadores Críticos.\n"
             << "0. Sair.\n"
             << "================================================================\n"
             << "Escolha uma opção: ";
        cin >> opcao;
        cin.ignore();
        if (opcao == 1 || opcao == 2 || opcao == 3 || opcao == 4 || opcao == 0) {
            return opcao;
        }
        cout << "\x1b[1;38;5;221;48;5;88mOpção inválida. Tente novamente.\x1b[0m\n";
    }
}

int main(int argc, char* argv[]) {
    graph::Graphroute<string> graph; 
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

    cout << "Grafo de roteamento inicializado" << "\n"
         << "Vértices únicos (IPs): "
         << graph.vertex_count()
         << " | Arestas: "
         << graph.edge_count()
         << "\n";

    int opcao;
    do {
        opcao = menu();


        
        
    switch (opcao) {
        case 1:
            graph.show();
            break;
        case 2:
            //TODO encontrar_menor_caminho();
            break;
        case 3:
            //TODO calcular_diametro_grafo();
            break;
        case 4:
            //TODO identificar_roteadores_criticos();
            break;
        case 0:
            break;
    }
    } while (opcao != 0);
    
    
    



    arq.close();

    return 0;

}