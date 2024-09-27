#include <cassert>
#include <string>
#include <iostream>
#include <limits>

#include "./src/heap.cpp"
#include "./src/HashTable.cpp"

using namespace std;

struct key {
    int id;
    int precio;
};

int compareKey(key k1, key k2){
    if(k1.precio > k2.precio) return 1;
    if(k1.precio < k2.precio) return -1;

    if(k1.id > k2.id) return -1;
    if(k1.id < k2.id) return 1;

    return 0;
}

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
    int quant;

    std::cin >> quant;

    Heap<key, int, void>* h = new Heap<key, int, void>(quant, compareKey, 0);

    for(int i = 0; i < quant; i++){
        int id;
        int precio;
        std::cin >> id;
        std::cin >> precio;
        key k = {
            .id = id,
            .precio = precio,
        };
        h->push(k, id);
    }

    std::cin >> quant;

    HashTable<int, int, void>* ids = new HashTable<int, int,void>(quant, equal, hash32shift, hash32shiftmult);

    for(int i = 0; i < quant; i++){
        int j = h->pop();
        if(ids->get(j) == NULL){
            std::cout << j << '\n';
            ids->set(j, j);
        }else{
            i--;
        }
    }

    return 0;
}
