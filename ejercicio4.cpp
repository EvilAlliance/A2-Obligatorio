#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <utility>

#include "./src/heap.cpp"
#include "./src/HashTable.cpp"

using namespace std;

struct key {
    int id;
    int prioridad;
    bool paraLlevar;
};

struct Order {
    int prioridad;
    bool paraLlevar;
    string items;
};

int compareKey(key k1, key k2){
    if(k1.prioridad < k2.prioridad ) return -1;
    if(k1.prioridad > k2.prioridad ) return 1;

    if(k1.paraLlevar && !k2.paraLlevar) return -1;
    if(!k1.paraLlevar && k2.paraLlevar) return 1;

    if(k1.id < k2.id) return -1;
    if(k1.id > k2.id) return 1;

    return 0;
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

bool equal(int a, int b){
    return a == b;
}

int main()
{
    int pedidos;
    std::cin >> pedidos;

    int operaciones;
    std::cin >> operaciones;

    Heap<key, int, void>* h = new Heap<key, int, void>(pedidos, compareKey, 0);
    HashTable<int, Order, void>* ids = new HashTable<int, Order,void>(pedidos, equal, hash32shift, hash32shiftmult);

    for(int i = 0; i < operaciones; i++){
        string op;
        std::cin >> op;
        if(op.compare("I") == 0){
            int id;
            int prioridad;
            string paraLlevarS;
            string p;

            std::cin >> id;
            std::cin >> prioridad;
            std::cin >> paraLlevarS;

            bool paraLlevar = 0;

            if(paraLlevarS.compare("true") == 0) paraLlevar = 1;

            key k = {
                .id = id,
                .prioridad = prioridad,
                .paraLlevar = paraLlevar
            };

            h->push(k, id);

            std::cin >> p;

            Order o = {
                .prioridad = prioridad,
                .paraLlevar = paraLlevar,
                .items = p
            };

            ids->set(id, o);
        }else if(op.compare("E") == 0){
            int id;
            std::cin >> id;

            ids->remove(id);
        }else if(op.compare("C") == 0){
            int id;
            std::cin >> id;

            for(unsigned long long i = 0; i < h->count; i++){
                key* k = &h->items[i].piority;
                if(k->id == id){
                    Order* o = ids->get(id);

                    k->paraLlevar = !k->paraLlevar;
                    o->paraLlevar = k->paraLlevar;

                   if(k->paraLlevar){
                       h->levitate(i);
                   }else{
                       h->sink(i);
                   }

                   break;
                }
            }

        }
    }

    while (h->count) {
        int id = h->pop();
        Order* o = ids->get(id);

        if(o != NULL){
            std::cout << id << " " << o->prioridad << " " << (o->paraLlevar ? "true" : "false") << " " << o->items << '\n';
        }
    }

    return 0;
}
