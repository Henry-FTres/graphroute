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
             << "Exibir como:\n"
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


auto path(graph::GraphRoute<string>& grafo)
{

    string start, end;
    cout << "IP de origem: ";
    getline(cin, start);
    cout << "IP de destino: ";
    getline(cin, end);

    auto path = grafo.shortest_path(start, end);
    if (path.empty()) {
        std::cout << "Nenhum caminho encontrado!\n";
    } else {
        cout << "Caminho encontrado!\n";
        for (auto p : path)
            std::cout << p->value << "->";

        std::cout << "\n";
        std::cout << "Número de saltos: " << path.size() - 1 << "\n"; //o número de saltos é o número de nós - 1, porque o caminho inclui o nó de origem.
    }
    return path;
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

    //SEPARANDO O NOME DO ARQUIVO DO PATH
    string nome_arquivo = argv[1];

    // remove o caminho (tudo antes da última '/')
    size_t pos_barra = nome_arquivo.find_last_of('/'); //size t guarda um tamanho, nesse caso vai gauardar a posição da última barra. Se não encontrar, retorna npos, que é um valor especial.
    if (pos_barra != string::npos) //se encontrou a barra, ou seja, se pos_barra não é npos
        nome_arquivo = nome_arquivo.substr(pos_barra + 1); //substr retorna uma substring, nesse caso a partir da posição da última barra + 1 (para não incluir a barra) até o final da string.

    // remove a extensão (tudo depois do último '.')
    size_t pos_ponto = nome_arquivo.find_last_of('.');
    if (pos_ponto != string::npos)
        nome_arquivo = nome_arquivo.substr(0, pos_ponto); //substr nesse caso retorna a substring do início da string até a posição do último ponto (sem incluir o ponto).
    
 
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
 
                    grafo.showPng(nome_arquivo);
                    break;
 
                case 3:
 
                    grafo.showDoc(nome_arquivo);
                    break;
 
                default:
                    break;
                }
            break;
        }
 
        case 2: {
           
            auto caminho = path(grafo);
           

            if (caminho.empty()) {
                break; // não mostra submenu se não achou caminho
            }

            opcSubmenu = submenu();
            switch (opcSubmenu) {
                case 1:
 
                    grafo.showScreenPath(caminho);
                    break;
               
                case 2:
 
                    grafo.showPngPath(nome_arquivo, caminho);
                    break;
 
                case 3:
 
                    grafo.showDocPath(nome_arquivo, caminho);
                    break;
 
                default:
                    break;
                }
            break;
        }
        case 3: {
 
            grafo.diameter();
            break;
        }
        case 4: {
            
            grafo.critical_routers();
            break;
        }
        case 0:
 
            break;
    }
    } while (opcao != 0);

   
   
 
    arq.close();
 
 
    return 0;
 
 
}


/*
TO DO:

- VERIFICAR QUANTOS CAMPOS SÃO
- VER SE PRECISA TRANSFORMAR O NOME DO ARQUIVO EM MAIÚSCULO (transform(nome_arquivo.begin(), nome_arquivo.end(), nome_arquivo.begin(), ::tolower); E INCLUIR ALGORITHM)









*/


