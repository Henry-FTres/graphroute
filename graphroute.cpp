#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>
 
namespace graph {
 
template<typename T>
class GraphRoute{
 
 
private:
 
    struct node{
        T value;
        std::unordered_set<node*> links;
        int inDegree = 0;
    };
 
 
    std::unordered_map<T, node> graph;

    int total_edges = 0;
 
 
    node* find(const T& val){
        auto it = graph.find(val);
        if (it == graph.end()) { //se chagar aqui quer dizer que não achou, por isso retorna nulo
            return nullptr;
        }
        return &it->second; //se encontrou retorna o nó
    }
 
 
    void insert_node(const T& val){
        if (graph.count(val) != 0)
            return;
        node aux;
        aux.value = val;
        graph[val] = aux;
    }
 
 
    void insert_link(const T& from, const T& to){
        auto pfrom = find(from);
        if (!pfrom)
            return;
        auto pto = find(to);
        if (!pto)
            return;
 
 
        if(pfrom->links.count(pto)==0){ //poderia ter verificado com o próprio insert pois ele retorna um std::pair<iterator,bool>, esse bool diz se inseriu ou se já tinha. Ficaria: if (pfrom->links.insert(pto).second) e incrementa os outros 2 dentro do if
            pfrom->links.insert(pto);
            pto->inDegree++;
            total_edges++;

        }
    }
 
    void gerar_dot() {
        std::ofstream dot("/tmp/graphviz.dot");//abre um arquivo ou cria se não existir
        dot << "digraph{\n";
        for (const auto& [key, node] : graph) {
            dot << "\t\"" << key << "\" -> {"; //"A" -> {
            for (const auto& link : node.links) {
                dot << "\"" << link->value << "\" ";
            }
            dot << "};\n";
        }
        dot << "}\n";
        dot.close();
    }

        
    void gerar_dot_path(const auto& caminho){

        //pra verificar rapidamente se o nó faz parte do caminho
        std::unordered_set<T> nos_caminho;
        for (auto p : caminho){
            nos_caminho.insert(p->value);
        }

        std::unordered_set<std::string> links_caminho;
        for (int i = 0; i < caminho.size() - 1; i++ ){
            links_caminho.insert(caminho[i]->value + "->" + caminho[i+1]->value);
        }

        std::ofstream dot("/tmp/graphviz.dot");//abre um arquivo ou cria se não existir
        dot << "digraph{\n";
        for (const auto& [key, node] : graph) {

            if(key == caminho[0]->value){ //value porque os ítens são nodes
                dot << "\t\"" << key << "\" [style=filled, fillcolor= \"#1a5c1a\", fontcolor=white];\n";
            }

             else if(key == caminho.back()->value){
                 dot << "\t\"" << key << "\" [style=filled, fillcolor=\"#cc0000\", fontcolor=white];\n";
            }

            else if(nos_caminho.count(key)!=0){
                dot << "\t\"" << key << "\" [style=filled, fillcolor=\"#4CAF50\", fontcolor=white];\n";
            }
           
            else{
                dot << "\t\"" << key << "\";\n";
            }

            for (const auto& link : node.links) {

                if(links_caminho.count(key + "->" + link->value) != 0){
                    dot << "\t\"" << key << "\" -> \"" << link->value << "\" [color=red, penwidth=2];\n";
                }
                else{
                    dot << "\t\"" << key << "\" -> \"" << link->value << "\";\n";
                }
            }
        }
        dot << "}\n";
        dot.close();
    }


public:

    //adiciona dois vértices e uma aresta que liga do from para o to
    void insert_edge(const T& hop_from, const T& hop_to) { 
        insert_node(hop_from);
        insert_node(hop_to);
        insert_link(hop_from, hop_to);
    }
 
    int vertex_count() {
        return graph.size();
    }

    int edge_count() {
        return total_edges;
    }
 
    void showScreen() {
        gerar_dot();
        if (system("dot -Tx11 /tmp/graphviz.dot") != 0) {
            std::cout << "Erro ao exibir o grafo. O graphviz está instalado?\n";
        }
    }
 
    void showPng(std::string nome_arquivo){
        gerar_dot();
        std::string command = "dot -Tpng /tmp/graphviz.dot -o ";
        command += nome_arquivo;
        command += ".png";
        if (system(command.c_str()) == 0) {
            std::cout << "Arquivo " << nome_arquivo << ".png gerado com sucesso\n";
        }
    }
 
    void showDoc(std::string nome_arquivo) {
        gerar_dot();
        std::string command = "dot -Tpdf /tmp/graphviz.dot -o ";
        command += nome_arquivo;
        command += ".pdf";
        if (system(command.c_str()) == 0) {
            std::cout << "Arquivo " << nome_arquivo << ".pdf gerado com sucesso\n";
        }
    }

    void showScreenPath(const std::vector<node*>& caminho) {
        gerar_dot_path(caminho);
        system("dot -Tx11 /tmp/graphviz.dot"); //dot = programa, Tx11 = opção que define o tipo da saída, /tmp/graphviz.dot = arquivo de entrada.
    }
 
    void showPngPath(std::string nome_arquivo, const std::vector<node*>& caminho) {
        gerar_dot_path(caminho);
        std::string command = "dot -Tpng /tmp/graphviz.dot -o ";
        command += nome_arquivo;
        command += "_path";
        command += ".png";
        if (system(command.c_str()) == 0) {
            std::cout << "Arquivo " << nome_arquivo << "_path.png gerado com sucesso\n";
        }
    }
 
    void showDocPath(std::string nome_arquivo, const std::vector<node*>& caminho) {
        gerar_dot_path(caminho);
        std::string command = "dot -Tpdf /tmp/graphviz.dot -o ";
        command += nome_arquivo;
        command += "_path";
        command += ".pdf";
        if (system(command.c_str()) == 0) {
            std::cout << "Arquivo " << nome_arquivo << "_path.pdf gerado com sucesso\n";
        }
    }
   

    std::vector<node *> shortest_path(const T& start, const T& end){
        std::vector<node *> path;
 
        // verifica se ip start e end existem no grafo, caso contrario retorna path vazio.
        auto pstart = find(start);
        if (!pstart)
            return path;
 
        auto pend = find(end);
        if (!pend)
            return path;
        
        // define fila de nós para serem visitados
        std::queue<node*> queue;
        // define set de visitados, pra não processar e visitar mesmo node
        std::unordered_set<node*> visited;
        // define map de origem. Pra cada node, guarda de onde ele veio, qual node descobriu este. Pra reconstruir o caminho depois
        std::unordered_map<node*, node*> origin;
        // bota pstart na fila e 
        queue.push(pstart);
        //marca ele como visitado 
        visited.insert(pstart);
        // dizendo que sua origem é nula
        origin[pstart] = nullptr;
        bool found = false;
 
        // enquanto a fila não estiver vazia
        while (!queue.empty()) {
            // define current como o nó em frente na fila e remove da fila
            auto current = queue.front();
            queue.pop();
            // se current for end (destino), marca encontrou como true e sai do loop pra nao procurar mais.
            if (current == pend){
                found = true;
                break;
            }
            // caso contrario, verifica os links de current (node na frente da fila).
            for (auto adj : current->links) {
                // pra cada link que ainda não foi visitado (enfileirado)
                if (visited.count(adj) == 0) {
                    queue.push(adj);
                    visited.insert(adj);
                    // registra a origem de cada link adjacente não visitado igual ao current (nodo que estamos lidando agora)
                    origin[adj] = current;
                }
            }            
        }
        // se encontrou for true
        if(found) {
            // define p como pEnd (nodo destino)
            auto p = pend;
            // enquanto p não for nulo
            while(p){
                // dá push back no vector para reconstruir o caminho
                path.push_back(p);
                // e define p como a origem dele mesmo, e repete o while até que origin[p] seja nulo, ou seja, seja pStart
                p = origin[p];
            }
        }
        // inverte o caminho porque ele foi reconstruido de tras pra frente
        std::reverse(path.begin(), path.end());
 
        return path;
    }

    int highest_distance(node* pstart) {

        std::queue<node*> queue;
        std::unordered_set<node*> visited;
        std::unordered_map<node*, int> distance;
        queue.push(pstart);
        visited.insert(pstart);
        distance[pstart] = 0;
 
        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();
            
            int current_distance = distance[current];
            for (auto adj : current->links) {
                if (visited.count(adj) == 0) {
                    queue.push(adj);
                    visited.insert(adj);
                    distance[adj] = current_distance + 1; // se fosse ++. modificaria o valor original no mapa
                }
            }            
        }
        
        int highest_hops = 0;
        for (auto& [key, max_hops] : distance) {
            if (max_hops > highest_hops) {
                highest_hops = max_hops;
            }
        }
        return highest_hops;
    }

    int diameter() {

        int result = 0;
        for (auto& [key, node] : graph) {
            auto p = &node;
            int max_distance = highest_distance(p);
            if (max_distance > result) {
                result = max_distance;
            }
        }

        return result;
    }

    void critical_routers() {

        if (graph.empty()) {
            std::cout << "Grafo vazio. Nenhum roteador para analisar.\n";
            return;
        }

        // cria o vector de pares (IP + inDegree)
        std::vector<std::pair<T, int>> vet;
        vet.reserve(graph.size());
        // percorre os nós e armazena o grau de entrada no vetor
        for (auto& [key, node] : graph) {
            vet.push_back({key, node.inDegree});
        }

        // quantos nodos vamos exibir, se é 5 ou menos, caso o grafo seja menor que 5
        const int k = std::min<int>(5, vet.size());

        // função de partial sort até vet.begin + k (5)
        std::partial_sort(vet.begin(), vet.begin() + k, vet.end(),
        [](const std::pair<T, int>& a, const std::pair<T, int>& b) {
            return a.second > b.second;
        });

        // exibe o top 5 mostrando ip e indegree
        for (int i = 0; i < k; i++) {
            std::cout << i << ". " << vet[i].first << " || "<< "In-degree: " << vet[i].second << "\n";
        }
    }

};
}
