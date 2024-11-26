#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

#include "./HashTable.cpp"

template <class K, class V> struct HashHeap_Cell {
  V value;
  K piority;
  HashHeap_Cell(K k, V v) : piority(k), value(v) {}
  HashHeap_Cell() : piority((K){0}), value((V){0}) {}
};

template <class K, class V, class X> class HashHeap {
public:
  HashHeap_Cell<K, V> *items;
  unsigned long long count = 0;
  unsigned long long capacity;

  /**
   * -1 cunado el primero es menor
   *  0 cunado son iguales
   *  1 cuando el primero es mayor
   * */
  int (*comparePiority)(K, K);
  int expectedRoot;

  X *data;
  void (*insertHandler)(K, V *, X *);
  void (*removeHandler)(K, V, X *);

  HashTable<K, unsigned long long, void>* h;

  HashHeap(unsigned long long len, int (*comparepiority)(K, K), bool maxHeap,
       bool(*e)(K, K), unsigned long long (*f)(K), unsigned long long (*s)(K),
       X *d, void (*inserthandler)(K, V *, X *) = NULL,
       void (*removehandler)(K, V, X *) = NULL) {
    items = new HashHeap_Cell<K, V>[len]();
    comparePiority = comparepiority;
    expectedRoot = maxHeap ? 1 : -1;
    capacity = len;

    data = d;
    insertHandler = inserthandler;
    removeHandler = removehandler;

    h = new HashTable<K, unsigned long long, void>(len,e, f, s);
  }

  HashHeap(unsigned long long len, int (*comparepiority)(K, K), bool maxHeap,
       bool(*e)(K, K), unsigned long long (*f)(K), unsigned long long (*s)(K),
       void (*inserthandler)(K, V *, X *) = NULL,
       void (*removehandler)(K, V, X *) = NULL) {
    items = new HashHeap_Cell<K, V>[len]();
    comparePiority = comparepiority;
    expectedRoot = maxHeap ? 1 : -1;
    capacity = len;

    insertHandler = inserthandler;
    removeHandler = removehandler;

    h = new HashTable<K, unsigned long long, void>(len, e, f, s);
  }

  ~HashHeap() {}

  unsigned long long childsOfLetf(unsigned long long i) { return 2 * i + 1; }

  unsigned long long childsOfRight(unsigned long long i) { return 2 * i + 2; }

  unsigned long long parentOf(unsigned long long i) {
    return floor((float)(i - 1) / 2.0);
  }

  void sink(unsigned long long i) {
    assert(i >= 0);
    if (i == count - 1)
      return;

    unsigned long long cL = childsOfLetf(i);
    unsigned long long cR = cL + 1;
    while (cL < count) {
      if (cR < count && comparePiority(items[cL].piority, items[cR].piority) !=
                            expectedRoot) {
        cL = cR;
      }
      if (comparePiority(items[cL].piority, items[i].piority) == expectedRoot) {
        HashHeap_Cell<K, V> temp = items[i];
        items[i] = items[cL];
        items[cL] = temp;

        h->set(items[i].piority, i);
        h->set(temp.piority, cL);
      }
      i = cL;
      cL = childsOfLetf(cL);
      cR = cL + 1;
    }
  }

  void levitate(unsigned long long i) {
    assert(i >= 0);

    unsigned long long p;

    while (i != 0 && comparePiority(items[(p = parentOf(i))].piority,
                                    items[i].piority) != expectedRoot) {
      HashHeap_Cell<K, V> temp = items[i];
      items[i] = items[p];
      items[p] = temp;

      h->set(temp.piority, p);
      h->set(items[i].piority, i);

      i = p;
    }
  }

  void push(K piority, V val) {
    assert(count <= capacity);
    HashHeap_Cell<K, V> *c = &items[count];

    h->set(piority, count);

    c->piority = piority;
    c->value = val;

    levitate(count);

    count++;

    if (insertHandler != NULL)
      insertHandler(piority, &val, data);
  }

  V pop() {
    HashHeap_Cell<K, V> c = items[0];

    h->remove(c.piority);

    count--;
    items[0] = items[count];

    h->set(items[0].piority, 0);

    sink(0);

    if (removeHandler != NULL)
      removeHandler(c.piority, c.value, data);

    return c.value;
  }

  V *peek() { 
    if(count == 0) return NULL;
    return &items[0].value;
  }

  V replaceTop(K piority, V val) {
    HashHeap_Cell<K, V> c = items[0];

    h->remove(c.piority);

    
    HashHeap_Cell<K, V> *cell = &items[0];
    cell->piority = piority;
    cell->value = val;

    h->set(piority, 0);

    sink(0);

    return c.value;
  }

  V *get(K k) {
    unsigned long long* x = h->get(k);
    if(x == NULL) return NULL;
    return &items[*x].value;
  }

  HashHeap_Cell<K, V>* getCell(K k) {
    unsigned long long* x = h->get(k);
    if(x == NULL) return NULL;
    return &items[*x];
  }

  void balance(K k){
    unsigned long long* i = h->get(k);
    if(i == NULL) return;
    sink(*i);
    levitate(*i);
  }

  void reset(){
    count = 0;
  }
};
