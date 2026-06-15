#include <iostream>
#include <fstream>
#include "graphroute.cpp"

using namespace std;

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
    graph::Graphroute<string> grafo; 
    if (argc != 2) {
        cerr << "Quantidade incorreta de argumentos! Esperado: 2" << "\n";
        return 1;
    }

    ifstream arq(argv[1]);

    if(!arq.is_open()) {
        cerr << "Erro ao abrir arquivo. Passe um caminho válido!" << "\n";
        return 1;
    }

    cout << "Grafo de roteamento inicializado" << "\n"
         << "Vértices únicos (IPs): " 
         //<< todo count_vertices()
         << " | Arestas: " 
         //<< todo count_edges()
         << "\n";

    int opcao;
    do {
        opcao = menu();
        
        switch (opcao) {
        case 1:
            grafo.show();
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