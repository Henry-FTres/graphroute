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
        system("dot -Tx11 /tmp/graphviz.dot"); //dot = programa, Tx11 = opção que define o tipo da saída, /tmp/graphviz.dot = arquivo de entrada.
    }
 
    void showPng(string nome_arquivo){ {
        gerar_dot();
        std::string command = "dot -Tpng /tmp/graphviz.dot -o ";
        command += nome_arquivo;
        command += ".png";
        system(command.c_str());
    }
 
    void showDoc(string nome_arquivo) {
        gerar_dot();
        std::string command = "dot -Tpdf /tmp/graphviz.dot -o ";
        command += nome_arquivo;
        command += ".pdf";
        system(command.c_str()); //c_str converte a string C++ para o formato que system() aceita.
    }

    void showScreenPath(const std::vector<node*>& caminho) {
        gerar_dot_path(caminho);
        system("dot -Tx11 /tmp/graphviz.dot"); //dot = programa, Tx11 = opção que define o tipo da saída, /tmp/graphviz.dot = arquivo de entrada.
    }
 
    void showPngPath(string nome_arquivo, const std::vector<node*>& caminho) {
        gerar_dot_path(caminho);
        std::string command = "dot -Tpng /tmp/graphviz.dot -o ";
        command += nome_arquivo;
        command += "_path";
        command += ".png";
        system(command.c_str());
    }
 
    void showDocPath(string nome_arquivo, const std::vector<node*>& caminho) {
        gerar_dot_path(caminho);
        std::string command = "dot -Tpdf /tmp/graphviz.dot -o ";
        command += nome_arquivo;
        command += "_path";
        command += ".pdf";
        system(command.c_str()); //c_str converte a string C++ para o formato que system() aceita.
    }
   

    std::vector<node *> shortest_path(const T& start, const T& end){
        std::vector<node *> path;
 
        auto pstart = find(start);
        if (!pstart)
            return path;
 
        auto pend = find(end);
        if (!pend)
            return path;
 
        std::queue<node*> queue;
        std::unordered_set<node*> queued;
        std::unordered_map<node*, node*> origin;
        queue.push(pstart);
        queued.insert(pstart);
        origin[pstart] = nullptr;
        bool found = false;
 
        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();
            if (current == pend){
                found = true;
                break;
            }
            for (auto adj : current->links) {
                if (queued.count(adj) == 0) {
                    queue.push(adj);
                    queued.insert(adj);
                    origin[adj] = current;
                }
            }            
        }
        if(found) {
            auto p = pend;
            while(p){
                path.push_back(p);
                p = origin[p];
            }
        }
        std::reverse(path.begin(), path.end());
 
        return path;
    }

    std::vector<node *> diameter(){
        std::vector<node *> path;
 
        auto pstart = find(start);
        if (!pstart)
            return path;
 
        auto pend = find(end);
        if (!pend)
            return path;
 
        std::queue<node*> queue; //precisam ser visitados
        std::unordered_set<node*> queued; //já foram visitados
        std::unordered_map<node*, node*> origin; //origem, quem levou até cada nó
        queue.push(pstart);
        queued.insert(pstart);
        origin[pstart] = nullptr;
        bool found = false;
 
        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();
            if (current == pend){
                found = true;
                break;
            }
            for (auto adj : current->links) { //percorre as conexões
                if (queued.count(adj) == 0) {
                    queue.push(adj);
                    queued.insert(adj);
                    origin[adj] = current;
                }
            }            
        }
        if(found) {
            auto p = pend;
            while(p){
                path.push_back(p);
                p = origin[p];
            }
        }
        std::reverse(path.begin(), path.end());
 
        return path;
    }
 
};
}
 
 
