#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <limits>

#include "./src/SparseGraph.cpp"

using namespace std;

// Extraido https://gist.github.com/badboy/6267743
unsigned long long hash32shift(int key){
  unsigned long long k = key;
  k = ~k + (k << 15); // k = (k << 15) - k - 1;
  k = k ^ (k >> 12);
  k = k + (k << 2);
  k = k ^ (k >> 4);
  k = k * 2057; // k = (k + (k << 3)) + (k << 11);
  k = k ^ (k >> 16);
  return k;
}

// Extraido https://gist.github.com/badboy/6267743
unsigned long long hash32shiftmult(int key){
    unsigned long long k = key;
    int c2=0x27d4eb2d; // a prime or an odd constant
    k = (k ^ 61) ^ (k >> 16);
    k = k + (k << 3);
    k = k ^ (k >> 4);
    k = k * c2;
    k = k ^ (k >> 15);
    return k;
}

struct Mission{
    unsigned long long id;
    string name;
    unsigned long long idCiudad;
};

struct City{
    unsigned long long id;
    string name;
};

unsigned long long hM1(Mission a){
    return hash32shift(a.id);
}

unsigned long long hM2(Mission a){
    return hash32shiftmult(a.id);
}

unsigned long long hC1(City a){
    return hash32shift(a.id);
}

unsigned long long hC2(City a){
    return hash32shiftmult(a.id);
}

typedef LinkedList<unsigned long long>* MissionDependencies;

bool equalMission(Mission a, Mission b) {
    return a.id == b.id;
}

string printCity(City a) {
    return a.name;
}

bool equalCity(City a, City b) {
    return a.id == b.id;
}

int main(){
    unsigned long long cantMission;
    std::cin >> cantMission;

    Mission* m = new Mission[cantMission]();
    MissionDependencies* md = new MissionDependencies[cantMission]();

    for(unsigned long long i = 0; i < cantMission; i++){
        unsigned long long id;
        string name;
        unsigned long long idCiudad;

        std::cin >> id;
        std::cin >> name;
        std::cin >> idCiudad;

        m[i] = {
            .id = id,
            .name = name,
            .idCiudad = idCiudad,
        };
        md[i] = new LinkedList<unsigned long long>();

        int  dep;
        std::cin >> dep;

        while(dep != 0){
            md[i]->push(dep);

            std::cin >> dep;
        }
    }

    SparseGraph<Mission>* gm = new SparseGraph<Mission>(5, equalMission, hM1, hM2);

    for(unsigned long long i = 0; i < cantMission; i++){
        gm->addVertex(m[i]);
    }

    for(unsigned long long i = 0; i < cantMission; i++){
       LinkedListIterator<unsigned long long>* it = md[i]->iterate();
        Mission org = {.id = i+1};
        while(it->hasNext()){
            unsigned long long* val = it->next();
            Mission dest = {.id = *val};

            gm->addEdge(org, dest, 1);
        }
    }


    SparseGraph<City>* gc = new SparseGraph<City>(5, equalCity, hC1, hC2);

    unsigned long long cantCiudades;
    unsigned long long org;

    std::cin >> cantCiudades;
    std::cin >> org;

    for(unsigned long long i = 0; i < cantCiudades; i++){
        unsigned long long id;
        string name;

        std::cin >> id;
        std::cin >> name;

        City c = {
            .id = id,
            .name = name,
        };

        gc->addVertex(c);
    }

    unsigned long long cantAristas;

    std::cin >> cantAristas;

    for(unsigned long long i = 0; i < cantAristas; i++){
        City org;
        City dest;
        long long cost;

        std::cin >> org.id;
        std::cin >> dest.id;
        std::cin >> cost;

        gc->addEdge(org, dest, cost);
        gc->addEdge(dest, org, cost);
    }


    DijkstraResult<City>* orgD = gc->dijkstra({.id = org});

    DijkstraResult<City>** mis = new DijkstraResult<City>*[gm->count];


    for (unsigned long long i = 0; i < gm->count; i++) {
        Mission m = gm->vertex[i];

        City c = {.id = m.idCiudad};

        mis[i] = gc->dijkstra(c);
    }

    unsigned long long* in = new unsigned long long[gm->count]();

    for (unsigned long long i = 0; i < gm->count; i++) {
        if(gm->edge[i] == NULL)continue;
        LinkedListIterator<Edge>* it = gm->edge[i]->iterate();
        while (it->hasNext()) {
            Edge* e = it->next();

            in[e->to]++;
        }

        delete it;
    }

    Heap<unsigned long long, unsigned long long, void>* h = 
        new Heap<unsigned long long, unsigned long long, void>
        (gm->count, compareCost, false);

    for (unsigned long long i = 0; i < gm->count; i++){
        if(in[i] == 0){
            unsigned long long j;
            assert(gc->findVertex({.id = gm->vertex[i].idCiudad},&j));
            h->push(orgD->dist[j],i);
        } 
    }

    unsigned long long cityAct;
    assert(gc->findVertex({.id=org}, &cityAct));

    bool f = true;
    unsigned long long misIndexAnt = 0;
    unsigned long long tiempo = 0;

    std::cout << "Ciudad inicial: " << gc->vertex[cityAct].name << "\n";

    while(h->peek() != NULL){
        unsigned long long i = h->pop();
 

        //Hice Mision

        assert(gc->findVertex({.id = gm->vertex[i].idCiudad},&cityAct));

        if(f){
            gc->printPath(orgD, gc->vertex[cityAct],printCity);
        } else {
            gc->printPath(mis[misIndexAnt], gc->vertex[cityAct],printCity);
        }

        tiempo += f ? orgD->dist[cityAct] : mis[misIndexAnt]->dist[cityAct];

        std::cout << "Mision: " << gm->vertex[i].name << " - " << gc->vertex[cityAct].name<< " - Tiempo de viaje: "<< (f ? orgD->dist[cityAct] : mis[misIndexAnt]->dist[cityAct])<< "\n";

        misIndexAnt = i;
        f = false;


        {
            LinkedList<unsigned long long>* l = new LinkedList<unsigned long long>();

            while(h->peek() != NULL)
               l->push(h->pop()) ;

            LinkedListIterator<unsigned long long>* it = l->iterate();
            while (it->hasNext()) {
                unsigned long long e = *it->next();

                h->push(mis[i]->dist[e], e);
            }

            delete it;
        }


        if(gm->edge[i] == NULL) continue;
        LinkedListIterator<Edge>* it = gm->edge[i]->iterate();
        while (it->hasNext()) {
            Edge* e = it->next();

            if (--in[e->to] == 0) {
                unsigned long long j;
                assert(gc->findVertex({.id = gm->vertex[e->to].idCiudad},&j));
                h->push(mis[i]->dist[j], e->to);
            }
        }
    }

    std::cout << "Misiones ejecutadas con exito.\nTiempo total de viaje: " << tiempo << "\n";

    return 0;
}
