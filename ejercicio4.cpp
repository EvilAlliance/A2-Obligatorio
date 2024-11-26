#include <cassert>
#include <iostream>
#include <limits>
#include <string>
#include <utility>

#include "./src/HashHeap.cpp"

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

int compareKey(key k1, key k2) {
  if (k1.prioridad < k2.prioridad)
    return -1;
  if (k1.prioridad > k2.prioridad)
    return 1;

  if (k1.paraLlevar && !k2.paraLlevar)
    return -1;
  if (!k1.paraLlevar && k2.paraLlevar)
    return 1;

  if (k1.id < k2.id)
    return -1;
  if (k1.id > k2.id)
    return 1;

  return 0;
}

// Extraido https://gist.github.com/badboy/6267743
unsigned long long hash32shift(int key) {
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
unsigned long long hash32shiftmult(int key) {
  unsigned long long k = key;
  int c2 = 0x27d4eb2d; // a prime or an odd constant
  k = (k ^ 61) ^ (k >> 16);
  k = k + (k << 3);
  k = k ^ (k >> 4);
  k = k * c2;
  k = k ^ (k >> 15);
  return k;
}

bool equal(int a, int b) { return a == b; }

// Extraido https://gist.github.com/badboy/6267743
unsigned long long hash32shift1(key key) {
  unsigned long long k = key.id;
  k = ~k + (k << 15); // k = (k << 15) - k - 1;
  k = k ^ (k >> 12);
  k = k + (k << 2);
  k = k ^ (k >> 4);
  k = k * 2057; // k = (k + (k << 3)) + (k << 11);
  k = k ^ (k >> 16);
  return k;
}

// Extraido https://gist.github.com/badboy/6267743
unsigned long long hash32shiftmult1(key key) {
  unsigned long long k = key.id;
  int c2 = 0x27d4eb2d; // a prime or an odd constant
  k = (k ^ 61) ^ (k >> 16);
  k = k + (k << 3);
  k = k ^ (k >> 4);
  k = k * c2;
  k = k ^ (k >> 15);
  return k;
}

bool equal1(key a, key b) { return a.id == b.id; }

int main() {
  int pedidos;
  std::cin >> pedidos;

  int operaciones;
  std::cin >> operaciones;

  HashHeap<key, int, void> *h = new HashHeap<key, int, void>(
      pedidos, compareKey, 0, equal1, hash32shift1, hash32shiftmult1);
  HashTable<int, Order, void> *ids = new HashTable<int, Order, void>(
      pedidos, equal, hash32shift, hash32shiftmult);

  for (int i = 0; i < operaciones; i++) {
    string op;
    std::cin >> op;
    if (op.compare("I") == 0) {
      int id;
      int prioridad;
      string paraLlevarS;
      string p;

      std::cin >> id;
      std::cin >> prioridad;
      std::cin >> paraLlevarS;

      bool paraLlevar = 0;

      if (paraLlevarS.compare("true") == 0)
        paraLlevar = 1;

      key k = {.id = id, .prioridad = prioridad, .paraLlevar = paraLlevar};

      h->push(k, id);

      std::cin >> p;

      Order o = {.prioridad = prioridad, .paraLlevar = paraLlevar, .items = p};

      ids->set(id, o);
    } else if (op.compare("E") == 0) {
      int id;
      std::cin >> id;

      ids->remove(id);
    } else if (op.compare("C") == 0) {
      key k;
      std::cin >> k.id;

      HashHeap_Cell<key, int>* c = h->getCell(k);

      Order *o = ids->get(k.id);

      c->piority.paraLlevar = !c->piority.paraLlevar;
      o->paraLlevar = c->piority.paraLlevar;

      h->balance(k);
    }
  }

  while (h->count) {
    int id = h->pop();
    Order *o = ids->get(id);

    if (o != NULL) {
      std::cout << id << " " << o->prioridad << " "
                << (o->paraLlevar ? "true" : "false") << " " << o->items
                << '\n';
    }
  }

  return 0;
}
