#include <cassert>
#include <string>
#include <iostream>
#include <limits>

#include "./src/SparseGraph.cpp"

using namespace std;

struct City {
    unsigned long long id;
    string name;
};

string printCity(City a) {
    return a.name;
}

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

unsigned long long hC1(City a){
    return hash32shift(a.id);
}

unsigned long long hC2(City a){
    return hash32shiftmult(a.id);
}

bool equalCity(City a, City b) {
    return a.id == b.id;
}

void updatePath(SparseGraph<City>* g, DijkstraResult<City>* p, City dest){
    unsigned long long indexOrg;
    assert(g->findVertex(dest, &indexOrg));

    if(p->parent[indexOrg] == NULL) return;

    unsigned long long indexDest;
    assert(g->findVertex(*p->parent[indexOrg], &indexDest));

    LinkedListIterator<Edge>* itOrg = g->edge[indexOrg]->iterate();

    while (itOrg->hasNext()) {
        Edge* e = itOrg->next();

        if(e->to == indexDest) {
            e->cost *=2;
            break;
        }
    }

    delete itOrg;

    LinkedListIterator<Edge>* itDest = g->edge[indexDest]->iterate();

    while (itDest->hasNext()) {
        Edge* e = itDest->next();

        if(e->to == indexOrg) {
            e->cost *=2;
            break;
        }
    }

    delete itOrg;

    updatePath(g, p, *p->parent[indexOrg]);
}


int main()
{
    unsigned long long cantVertex;
    std::cin >> cantVertex;

    SparseGraph<City>* g = new SparseGraph<City>(cantVertex, equalCity, hC1, hC2);

    for (unsigned long long i = 0; i < cantVertex; i++) {
        City c;

        std::cin >> c.id;
        std::cin >> c.name;

        g->addVertex(c);
    }

    unsigned long long idStart;
    unsigned long long idEntity;
    unsigned long long idTeam;
    unsigned long long idPoint;

    std::cin >> idStart;
    std::cin >> idEntity;
    std::cin >> idTeam;
    std::cin >> idPoint;

    unsigned long long cantEdge;
    std::cin >> cantEdge;

    for (unsigned long long i = 0; i < cantEdge; i++) {
        City org;
        City dest;
        long long cost;

        std::cin >> org.id;
        std::cin >> dest.id;
        std::cin >> cost;

        g->addEdge(org, dest, cost);
        g->addEdge(dest, org, cost);
    }

    unsigned long long indexIdStart;
    assert(g->findVertex({.id = idStart}, &indexIdStart));

    unsigned long long indexIdEntity;
    assert(g->findVertex({.id = idEntity}, &indexIdEntity));

    unsigned long long indexIdTeam;
    assert(g->findVertex({.id = idTeam}, &indexIdTeam));

    unsigned long long indexIdPoint;
    assert(g->findVertex({.id = idPoint}, &indexIdPoint));

    SparseGraph<City>* gg = g->clone();
    SparseGraph<City>* gd = g->clone();

    // First deactivate entity
    DijkstraResult<City>* dDeactivateEntity = gd->dijkstra({.id = idStart});
    updatePath(gd, dDeactivateEntity, {.id = idEntity});
    DijkstraResult<City>* dGetTeam = gd->dijkstra({.id = idEntity});
    DijkstraResult<City>* dGoToPoint = gd->dijkstra({.id = idTeam});

    long long dCost = dDeactivateEntity->dist[indexIdEntity] + dGetTeam->dist[indexIdTeam] + dGoToPoint->dist[indexIdPoint];

    // First get team
    DijkstraResult<City>* gGetTeam = gg->dijkstra({.id = idStart});
    updatePath(gg, gGetTeam, {.id = idTeam});
    DijkstraResult<City>* gDeactivateEntity = gg->dijkstra({.id = idTeam});
    updatePath(gg, gDeactivateEntity, {.id = idEntity});
    DijkstraResult<City>* gGoToPoint = gg->dijkstra({.id = idEntity});
    long long gCost = gDeactivateEntity->dist[indexIdEntity] + gGetTeam->dist[indexIdTeam] + gGoToPoint->dist[indexIdPoint];

    if(dCost <= gCost) {
        std::cout << "BE11, la mejor ruta es Desactivar la Entidad, buscar equipo y punto de extraccion con un costo de " << dCost << '\n';
        std::cout << "La otra opcion tiene un costo de " << gCost << '\n';
        std::cout << "Paso 1: ";
        g->printPath(dDeactivateEntity, g->vertex[indexIdEntity], printCity);
        std::cout << "Desactivar la Entidad\n";
        std::cout << "Paso 2: ";
        g->printPath(dGetTeam, g->vertex[indexIdTeam], printCity);
        std::cout << "Buscar equipo\n";
        std::cout << "Paso 3: ";
        g->printPath(dGoToPoint, g->vertex[indexIdPoint], printCity);
        std::cout << "Ir a Punto de extraccion\n";
    }else {
        std::cout << "BE11, la mejor ruta es buscar equipo, Desactivar la Entidad y punto de extraccion con un costo de " << gCost << "\n";
        std::cout << "La otra opcion tiene un costo de " << dCost << '\n';
        std::cout << "Paso 1: ";
        g->printPath(gGetTeam, g->vertex[indexIdTeam], printCity);
        std::cout << "Buscar equipo\n";
        std::cout << "Paso 2: ";
        g->printPath(gDeactivateEntity, g->vertex[indexIdEntity], printCity);
        std::cout << "Desactivar la Entidad\n";
        std::cout << "Paso 3: ";
        g->printPath(gGoToPoint, g->vertex[indexIdPoint], printCity);
        std::cout << "Ir a Punto de extraccion\n";
    }

    return 0;
}
