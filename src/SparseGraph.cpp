#include "./LinkedList.cpp"
#include "./HashTable.cpp"
#include "./heap.cpp"
#include <memory>
#include <sched.h>
#include <string>
#include <limits>

struct Edge{
    long long cost;
    unsigned long long to;
};


int compareCost(unsigned long long a, unsigned long long b){
    if(a < b) return -1;
    if(a > b) return 1;
    return 0;
}

template<class V> struct DijkstraResult {
    unsigned long long* dist;
    V** parent;
};

template<class V> class SparseGraph {
public:
    V* vertex;
    LinkedList<Edge>** edge;

    unsigned long long count;
    unsigned long long capacity;

    HashTable<V, unsigned long long, void>* h;

    SparseGraph(unsigned long long cap, bool(*e)(V, V), unsigned long long (*f)(V), unsigned long long (*s)(V)){
        count = 0;
        capacity = cap;

        vertex = new V[cap]();
        edge = new LinkedList<Edge>*[cap]();

        h = new HashTable<V, unsigned long long, void>(cap, e, f, s);
    }

    void expand(){
        capacity *= 2;

        V* oldVertex = vertex;
        LinkedList<Edge>** oldEdge = edge;

        vertex = new V[capacity]();
        edge = new LinkedList<Edge>*[capacity]();

        for(unsigned long long i = 0; i < count; i++){
            vertex[i] = oldVertex[i];
            edge[i] = oldEdge[i];
        }

        delete[] oldVertex;
        delete[] oldEdge;
    }

    bool findVertex(V v, unsigned long long* index){
        unsigned long long* i = h->get(v);
        *index = *i;

        return i != NULL;
    }

    void addVertex(V v){
        if(count >= capacity) expand();
        h->set(v, count);
        vertex[count] = v;
        count++;
    }

    void addEdge(V org, V dest, long long cost){
        unsigned long long from;
        unsigned long long to;

        assert(findVertex(org, &from) && findVertex(dest, &to));

        Edge e;
        e.to = to;
        e.cost = cost;

        if(edge[from] == NULL) edge[from] = new LinkedList<Edge>();

        edge[from]->push(e);
    }

    DijkstraResult<V>*  dijkstra(V src){
        unsigned long long indexSrc;
        assert(findVertex(src, &indexSrc));

        bool* visisted = new bool[count]();

        DijkstraResult<V>* r = new DijkstraResult<V>();
        r->dist = new unsigned long long[count];
        r->parent = new V*[count]();

        for(unsigned long long i = 0; i < count; i++){
            r->dist[i] = std::numeric_limits<unsigned long long>::max();
            r->parent[i] = NULL;
            visisted[i] = false;
        } 

        r->dist[indexSrc] = 0;

        Heap<unsigned long long, unsigned long long, void>* h = new Heap<unsigned long long, unsigned long long, void>(count*count, compareCost, false);

        h->push(0, indexSrc);

        while(h->peek() != NULL){
            unsigned long long i = h->pop();

            if(visisted[i]) continue;
            visisted[i] = true;

            if(edge[i] == NULL) continue;
            LinkedListIterator<Edge>* it = edge[i]->iterate();

            while(it->hasNext()) {
                Edge* e = it->next();

                if(!visisted[e->to] && r->dist[e->to] > r->dist[i] + e->cost) {
                    r->dist[e->to] = r->dist[i] + e->cost;
                    r->parent[e->to] = &vertex[i];

                    h->push(r->dist[e->to], e->to);
                }
            }

            delete it;
        }

        delete[] visisted;

        return r;
    }

    void printPath(DijkstraResult<V>* d, V dest, std::string(*p)(V)){
        unsigned long long i;
        assert(findVertex(dest,&i));
        if(d->parent[i] != NULL)
            printPath(d, *d->parent[i], p);
        
        std::cout << p(dest) << " -> ";
    }

    SparseGraph<V>* clone() {
        SparseGraph<V>* g = new SparseGraph<V>(count, h->equal, h->fhash, h->shash);

        for (unsigned long long i = 0; i < count; i++) {
            g->addVertex(vertex[i]);
        }

        for (unsigned long long i = 0; i < count; i++) {
            if(edge[i] == NULL) continue;
            LinkedListIterator<Edge>* it = edge[i]->iterate();

            while (it->hasNext()) {
                Edge* e = it->next();

                g->addEdge(vertex[i], vertex[e->to], e->cost);
            }

            delete it;
        }

        return g;
    }
};

