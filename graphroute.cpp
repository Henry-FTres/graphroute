#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <queue>
#include <vector>
 
namespace graph {
 
template<typename T>
class GraphRoute{
 
 
private:
 
    struct node{
        T value;
        std::unordered_set<node*> links;
    };
 
 
    std::unordered_map<T, node> graph;

    std::unordered_map<T, int> inDegrees;
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
            inDegrees[pto->value]++;
            total_edges++;

        }
    }
 
    void gerar_dot() {
        std::ofstream dot("/tmp/graphviz.dot");
        dot << "digraph{\n";
        for (const auto& [key, node] : graph) {
            dot << "\t\"" << key << "\" -> {";
            for (const auto& link : node.links) {
                dot << "\"" << link->value << "\" ";
            }
            dot << "};\n";
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
 
   
 
    void showScreen() {
        gerar_dot();
        system("dot -Tx11 /tmp/graphviz.dot");
    }
 
    void showPng(string current_path) {
        std::string command = "dot -Tpng /tmp/graphviz.dot -o ";
        command += current_path;
        command += ".png";
        gerar_dot();
        system(command.c_str());
    }
 
    void showDoc(string current_path) {
        std::string command = "dot -Tpdf /tmp/graphviz.dot -o ";
        command += current_path;
        command += ".pdf";
        gerar_dot();
        system(command.c_str());
    }
 
    int edge_count() {
        return total_edges;
    }
 
    std::vector<node *> shortest_path(const T& start, const T& end)
    {
        // define vector path
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
    
};
}   
 
 
