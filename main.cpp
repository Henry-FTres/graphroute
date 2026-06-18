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

void path(graph::GraphRoute<string>& grafo)
{

    string start, end;
    cout << "Inicial: ";
    getline(cin, start);
    cout << "Final: ";
    getline(cin, end);

    auto path = grafo.shortest_path(start, end);
    if (path.empty()) {
        std::cout << "Nenhum caminho encontrado!\n";
    } else {
        cout << "Caminho encontrado (" << path.size() << " saltos):\n";
        for (auto p : path)
            std::cout << p->value << "->";

        std::cout << "\n";
    }
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
        cout << "\x1b["          // Início do código ANSI
            << "1;"             // Negrito
            << "38;5;221;"      // Cor do texto
            << "48;5;88m"       // Cor do fundo
            << "Opção inválida. Tente novamente."
            << "\x1b[0m"        // Resetar formatação
            << "\n";
    }
}
 
int submenu() {
    int opcao;
    while (true) {
        cout << "================================================================\n"
             << "1. Tela.\n"
             << "2. Imagem (PNG).\n"
             << "3. Documento (PDF).\n"
             << "================================================================\n"
             << "Escolha uma opção: ";
        cin >> opcao;
        cin.ignore();
        if (opcao == 1 || opcao == 2 || opcao == 3) {
            return opcao;
        }
        cout << "\x1b[1;38;5;221;48;5;88mOpção inválida. Tente novamente.\x1b[0m\n";
    }
}
 
 
int main(int argc, char* argv[]) {
    graph::GraphRoute<string> grafo; 
   
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
    getline(arq,auxiliar); //para descartar a linha de cabeçalho
    while(getline(arq, auxiliar)){
        auto fields = split(auxiliar, ',');

        if(fields.size() < 6){
            continue;
        }
        else if(fields[5] == "*" || fields[4] == "" || fields[5] == ""){
            continue;
        }
        else{
            grafo.insert_edge(fields[4], fields[5]);
        }
    }
 
 
    cout << "Grafo de roteamento inicializado\n" 
         << "Vértices únicos (IPs): "
         << grafo.vertex_count()
         << " | Arestas: "
         << grafo.edge_count()
         << "\n";
 
 
    int opcao;
    int opcSubmenu;
    do {
        opcao = menu();
 
    switch (opcao) {
        case 1: {
           
            opcSubmenu = submenu();
            switch (opcSubmenu) {
                case 1:
 
                    grafo.showScreen();
                    break;
               
                case 2:
 
                    grafo.showPng(argv[1]);
                    break;
 
                case 3:
 
                    grafo.showDoc(argv[1]);
                    break;
 
                default:
                    break;
                }
            break;
        }
 
        case 2: {
           
            // TODO implementar submenu(), com as opções de mostrar o menor caminho em .dot, em pdf e em png, destacando o shortest path com colorido ****LER PONTO 2 MENU INTERATIVO NO DOC DO TRABALHO QUE O BRUSSO PASSOU
            path(grafo);
        }
        case 3: {
 
            //TODO calcular_diametro_grafo();
            break;
        }
        case 4: {
           
            //TODO identificar_roteadores_criticos();
            break;
        }
        case 0:
 
            break;
    }
    } while (opcao != 0);
   
   
   
 
    arq.close();
 
 
    return 0;
 
 
}
