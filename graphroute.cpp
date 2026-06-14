#include <unordered_map>
#include <unordered_set>

namespace graph{

template<typename T>
class Graphroute{

private:
    struct node{
        T value;
        std::unordered_set<node*> links;
    };

};
}