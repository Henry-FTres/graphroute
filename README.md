# GraphRoute — Analisador de Rotas de Rede com Grafos

- Erik Ferreira Rocha
- Henry Afonso Fiorentin Tres
- Manuela Larissa Stivanin

---

## O QUE É?

É um projeto desenvolvido para a disciplina de Estrutura de Dados II do curso de Ciência da Computação. O programa lê arquivos de log no formato `.log` gerados por ferramentas de traceroute, constrói um grafo direcionado em memória com os endereços IP encontrados e oferece um menu interativo para análise da topologia de rede.

As funcionalidades disponíveis são: exibição visual do grafo, busca de menor caminho entre dois IPs, cálculo do diâmetro do grafo e identificação dos roteadores mais críticos por grau de entrada.

---

## COMO USAR?

### Dependências

Para compilar e executar o projeto, é necessário ter:

- Compilador C++ com suporte ao padrão C++20
- Graphviz, para gerar e visualizar os grafos

No Linux, o compilador pode ser instalado com:

```bash
sudo apt install build-essential
```

Para verificar se o compilador foi instalado:

```bash
g++ --version
```

O Graphviz pode ser instalado com:

```bash
sudo apt install graphviz
```

Para verificar se a instalação do Graphviz funcionou:

```bash
dot -V
```

Se aparecer a versão instalada, a instalação foi concluída corretamente.

---

### Compilação

O projeto usa o padrão C++20, pois o código faz uso de structured bindings, como por exemplo:

```cpp
for (const auto& [key, node] : graph)
```

Para compilar:

```bash
g++ -std=c++20 main.cpp -o graphroute
```

Como o `main.cpp` já inclui o `graphroute.cpp` diretamente, a compilação é feita apenas com o `main.cpp`.

---

### Execução

```bash
./graphroute <caminho_do_arquivo.log>
```

Exemplo:

```bash
./graphroute input_1.log
```

O programa exige exatamente um argumento. Se nenhum arquivo for informado, ou se o arquivo não puder ser aberto, uma mensagem de erro será exibida e o programa encerrará.

---

## FORMATO DO ARQUIVO DE ENTRADA

O arquivo de entrada é um `.log` com valores separados por vírgula, no seguinte formato:

```
prb_id,probe_src,dst_addr,hop,hop_from,hop_to,rtt
```

As colunas utilizadas pelo programa são:

- `hop_from` (posição 4): IP de origem do salto, vértice de origem da aresta
- `hop_to` (posição 5): IP de destino do salto, vértice de destino da aresta

Cada linha válida representa uma aresta direcionada no grafo:

```
hop_from -> hop_to
```

Linhas com `hop_to` igual a `*`, campos `hop_from` ou `hop_to` em branco, ou linhas com menos de 6 campos são descartadas automaticamente.

---

## LEITURA DAS LINHAS DO ARQUIVO

Para separar os campos de cada linha, foi criada a função `split` no `main.cpp`:

```cpp
vector<string> split(const string& line, char sep) {
    vector<string> parts;
    stringstream stream(line);
    string aux;
    while (getline(stream, aux, sep)) {
        parts.push_back(aux);
    }
    return parts;
}
```

A função usa `stringstream` para tratar a linha como um fluxo de entrada. O separador passado é a vírgula:

```cpp
auto fields = split(auxiliar, ',');
```

Assim, cada linha do CSV é dividida em campos e armazenada num `vector<string>` para acesso por posição.

---

## VALIDAÇÃO DOS DADOS

A validação é feita no `main.cpp`, antes de qualquer inserção no grafo:

```cpp
if (fields.size() < 7) {
    continue;
}
else if (fields[5] == "*" || fields[4] == "" || fields[5] == "") {
    continue;
}
else {
    grafo.insert_edge(fields[4], fields[5]);
}
```

Linhas com campos insuficientes, IPs em branco ou com `*` são ignoradas. Apenas linhas completamente válidas chegam à estrutura do grafo.

---

## SEPARAÇÃO DO NOME DO ARQUIVO

Para nomear corretamente os arquivos de saída (PNG, PDF), o programa extrai apenas o nome base do arquivo de entrada, removendo o caminho e a extensão:

```cpp
size_t pos_barra = nome_arquivo.find_last_of('/');
if (pos_barra != string::npos)
    nome_arquivo = nome_arquivo.substr(pos_barra + 1);

size_t pos_ponto = nome_arquivo.find_last_of('.');
if (pos_ponto != string::npos)
    nome_arquivo = nome_arquivo.substr(0, pos_ponto);
```

Assim, ao passar `./dados/input_1.log`, o nome base extraído será `input_1`, e os arquivos gerados serão `input_1.png`, `input_1.pdf`, etc.

---

## MENU DO PROGRAMA

Após o carregamento, o programa exibe um menu interativo:

```
================================================================
1. Exibir grafo completo.
2. Encontrar menor caminho.
3. Calcular diâmetro do grafo.
4. Identificar Roteadores Críticos.
0. Sair.
================================================================
```

As opções 1 e 2 apresentam um submenu de formato de saída:

```
================================================================
Exibir como:
1. Tela.
2. Imagem (PNG).
3. Documento (PDF).
================================================================
```

Opções inválidas em qualquer menu exibem uma mensagem de erro em destaque com cores ANSI e repetem a pergunta.

---

## ORGANIZAÇÃO DO CÓDIGO

O projeto é dividido em dois arquivos:

- `graphroute.cpp`: contém a classe template `GraphRoute` com toda a estrutura do grafo e os algoritmos
- `main.cpp`: contém a leitura do arquivo, validação dos dados, menu interativo e chamadas às funções do grafo

Essa separação mantém a lógica do grafo isolada em um arquivo e a interação com o usuário em outro, facilitando a manutenção.

---

## GRAFO DIRECIONADO

O projeto usa um grafo direcionado porque cada linha válida do arquivo representa um salto com direção:

```
hop_from -> hop_to
```

Uma ligação de A para B não implica que exista uma ligação de B para A. Isso é importante porque as rotas de rede possuem sequência de saltos com direção definida.

---

## ESTRUTURA DO NÓ

Cada IP é representado como um nó do grafo. A estrutura utilizada foi:

```cpp
struct node {
    T value;
    std::unordered_set<node*> links;
};
```

O atributo `value` guarda o endereço IP do nó.

O atributo `links` guarda ponteiros para os nós vizinhos, ou seja, os IPs para os quais este nó aponta.

A classe é template em `T`, o que permite usar qualquer tipo como chave. No projeto, `T` é `std::string`.

---

## ARMAZENAMENTO DO GRAFO

O grafo é armazenado em um `unordered_map`:

```cpp
std::unordered_map<T, node> graph;
```

A chave é o endereço IP e o valor é o nó correspondente. Exemplo:

```
"192.168.0.1" -> node
"10.0.0.1"    -> node
```

Essa estrutura foi usada porque permite encontrar qualquer nó a partir do seu IP em tempo O(1) médio.

---

## POR QUE UNORDERED_SET NOS LINKS?

Os vizinhos de cada nó são armazenados com:

```cpp
std::unordered_set<node*> links;
```

O `unordered_set` não permite elementos repetidos. Assim, se a mesma aresta aparecer mais de uma vez no arquivo de log, ela é inserida apenas uma vez no grafo, sem necessidade de verificação manual extra na maioria dos casos.

---

## RASTREAMENTO DO GRAU DE ENTRADA

O grau de entrada de cada nó (in-degree) é armazenado separadamente num mapa:

```cpp
std::unordered_map<T, int> inDegrees;
```

Esse mapa é atualizado incrementalmente a cada chamada de `insert_link`:

```cpp
inDegrees[pto->value]++;
```

Manter o in-degree num mapa separado evita recalcular esse valor na hora da consulta, tornando a opção 4 eficiente independentemente do tamanho do grafo.

---

## INSERÇÃO DE NÓS

A função `insert_node` adiciona um IP como vértice do grafo:

```cpp
void insert_node(const T& val) {
    if (graph.count(val) != 0)
        return;
    node aux;
    aux.value = val;
    graph[val] = aux;
}
```

Ela verifica se o IP já existe antes de inserir. Se já existir, retorna sem fazer nada.

---

## INSERÇÃO DE ARESTAS

A função pública `insert_edge` é o ponto de entrada para inserção. Ela garante que ambos os nós existam e depois cria a aresta entre eles:

```cpp
void insert_edge(const T& hop_from, const T& hop_to) {
    insert_node(hop_from);
    insert_node(hop_to);
    insert_link(hop_from, hop_to);
}
```

Internamente, `insert_link` verifica se a aresta já existe antes de inserir:

```cpp
if (pfrom->links.count(pto) == 0) {
    pfrom->links.insert(pto);
    inDegrees[pto->value]++;
    total_edges++;
}
```

Somente se a aresta ainda não existir ela é inserida, o in-degree do destino é incrementado e o contador total de arestas é atualizado.

---

## MENOR CAMINHO

Para encontrar o menor caminho entre dois IPs, foi utilizada a busca em largura (BFS).

A BFS foi escolhida porque o grafo não possui pesos nas arestas. Nesse caso, o menor caminho é simplesmente aquele com a menor quantidade de saltos, e a BFS garante isso por natureza, visitando os nós por níveis.

A função utiliza três estruturas auxiliares:

- `queue`: controla a ordem de visita dos nós, por nível
- `unordered_set visited`: marca os nós já enfileirados para não processá-los novamente
- `unordered_map origin`: para cada nó, guarda de qual nó ele foi alcançado, permitindo reconstruir o caminho ao final

Durante a busca, o programa percorre os vizinhos do nó atual:

```cpp
for (auto adj : current->links) {
    if (visited.count(adj) == 0) {
        queue.push(adj);
        visited.insert(adj);
        origin[adj] = current;
    }
}
```

Quando o destino é encontrado, o caminho é reconstruído de trás para frente usando o mapa `origin`:

```cpp
auto p = pend;
while (p) {
    path.push_back(p);
    p = origin[p];
}
std::reverse(path.begin(), path.end());
```

O resultado é um `vector<node*>` com os nós na ordem correta do caminho.

---

## DIÂMETRO DO GRAFO

O diâmetro do grafo é o maior dentre todos os menores caminhos entre pares de nós alcançáveis.

A função `highest_distance` executa uma BFS a partir de um nó inicial e calcula a distância até todos os nós alcançáveis, retornando a maior delas:

```cpp
int current_distance = distance[current];
for (auto adj : current->links) {
    if (visited.count(adj) == 0) {
        queue.push(adj);
        visited.insert(adj);
        distance[adj] = current_distance + 1;
    }
}
```

A distância de cada adjacente é calculada como `current_distance + 1`, sem modificar o valor do nó pai no mapa. Isso é importante porque um mesmo nó pode ter múltiplos vizinhos — se fosse usado `++distance[current]`, o valor do pai seria alterado a cada iteração, corrompendo as distâncias dos demais vizinhos.

Ao final da BFS, o mapa `distance` é percorrido para encontrar o maior valor:

```cpp
int highest_hops = 0;
for (auto& [key, max_hops] : distance) {
    if (max_hops > highest_hops)
        highest_hops = max_hops;
}
```

A função `diameter` chama `highest_distance` para cada nó do grafo e retorna o maior resultado encontrado:

```cpp
for (auto& [key, node] : graph) {
    auto p = &node;
    int max_distance = highest_distance(p);
    if (max_distance > result)
        result = max_distance;
}
```

---

## ROTEADORES CRÍTICOS

Os roteadores críticos são identificados pelo grau de entrada (in-degree): quanto mais arestas chegam a um nó, mais central ele é na topologia da rede.

A função `critical_routers` monta um vetor de pares `(IP, in-degree)` a partir do mapa `inDegrees`:

```cpp
std::vector<std::pair<T, int>> vet;
for (auto& [key, val] : inDegrees) {
    vet.push_back({key, val});
}
```

Em seguida, usa `std::partial_sort` com uma lambda para ordenar apenas os `k` primeiros elementos do vetor pelo maior in-degree:

```cpp
const int k = std::min<int>(5, vet.size());

std::partial_sort(vet.begin(), vet.begin() + k, vet.end(),
    [](const std::pair<T, int>& a, const std::pair<T, int>& b) {
        return a.second > b.second;
    });
```

O `partial_sort` foi escolhido em vez do `sort` completo porque apenas os 5 primeiros elementos importam. Ele garante que as posições 0 a k-1 contenham os maiores valores ordenados, sem precisar ordenar o vetor inteiro — o que é mais eficiente em grafos grandes.

Por fim, os k roteadores são exibidos:

```cpp
for (int i = 0; i < k; i++) {
    std::cout << i << ". " << vet[i].first << " || In-degree: " << vet[i].second << "\n";
}
```

---

## VISUALIZAÇÃO COM GRAPHVIZ

As opções de visualização geram dinamicamente um arquivo `.dot` em `/tmp/graphviz.dot` e chamam o Graphviz via `system()` para produzir a saída escolhida.

Exemplo de comando gerado internamente:

```bash
dot -Tpng /tmp/graphviz.dot -o input_1.png
```

Na visualização do menor caminho, a função `gerar_dot_path` destaca os nós e arestas do caminho com cores diferenciadas:

- Nó de origem: verde escuro (`#1a5c1a`)
- Nós intermediários: verde claro (`#4CAF50`)
- Nó de destino: vermelho (`#cc0000`)
- Arestas do caminho: `color=red, penwidth=2`

Para isso, o programa monta dois `unordered_set`: um com os valores dos nós do caminho e outro com as arestas no formato `"A->B"`, permitindo verificar rapidamente se um nó ou aresta faz parte do caminho ao percorrer o grafo para geração do `.dot`.

---

## ARQUIVOS GERADOS

Ao exibir o grafo completo:

```
input_1.png
input_1.pdf
```

Ao exibir o menor caminho:

```
input_1_path.png
input_1_path.pdf
```

Os nomes são derivados automaticamente do nome do arquivo de entrada informado na execução.

---

## EXEMPLO DE USO

Compilar:

```bash
g++ -std=c++20 main.cpp -o graphroute
```

Executar:

```bash
./graphroute input_1.log
```

Saída inicial:

```
Grafo de roteamento inicializado
Vértices únicos (IPs): 342 | Arestas: 812
```

Exemplo de menor caminho:

```
IP de origem: 82.66.191.65
IP de destino: 194.149.162.250

Caminho encontrado!
82.66.191.65->192.168.3.1->194.149.162.248->194.149.162.250->
Número de saltos: 3
```

Exemplo de roteadores críticos:

```
0. 20.157.222.42 || In-degree: 7
1. 176.52.248.125 || In-degree: 3
2. 213.140.50.187 || In-degree: 3
3. 213.140.36.233 || In-degree: 2
4. 213.140.36.3 || In-degree: 2
```

---

## JUSTIFICATIVAS DAS ESCOLHAS

O `unordered_map` foi usado para armazenar os nós do grafo porque permite busca por IP em O(1) médio, sem necessidade de percorrer toda a estrutura.

O `unordered_set` foi usado nos links de cada nó para evitar arestas duplicadas automaticamente, já que conjuntos não permitem elementos repetidos.

O `inDegrees` foi mantido como um mapa separado, atualizado incrementalmente a cada inserção, para que a consulta dos roteadores críticos não precise recalcular os graus de entrada no momento da execução.

A `queue` foi usada na BFS porque ela processa os nós por ordem de chegada (FIFO), garantindo que os nós mais próximos da origem sejam visitados antes dos mais distantes — o que é exatamente o comportamento necessário para encontrar o menor caminho em um grafo sem pesos.

O `partial_sort` foi preferido ao `sort` completo na identificação dos roteadores críticos porque, quando só os 5 primeiros resultados importam, não há necessidade de ordenar o vetor inteiro.

A classe `GraphRoute` foi implementada como template para que a estrutura do grafo não fique amarrada ao tipo `string`. No projeto, `T` é sempre `string`, mas a classe poderia trabalhar com qualquer tipo comparável.