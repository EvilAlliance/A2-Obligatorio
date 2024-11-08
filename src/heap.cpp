#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

template <class K, class V> struct Heap_Cell {
  V value;
  K piority;
  Heap_Cell(K k, V v) : piority(k), value(v) {}
  Heap_Cell() : piority((K){0}), value((V){0}) {}
};

template <class K, class V, class X> class Heap {
public:
  Heap_Cell<K, V> *items;
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

  Heap(unsigned long long len, int (*comparepiority)(K, K), bool maxHeap, X *d,
       void (*inserthandler)(K, V *, X *) = NULL,
       void (*removehandler)(K, V, X *) = NULL) {
    items = new Heap_Cell<K, V>[len]();
    comparePiority = comparepiority;
    expectedRoot = maxHeap ? 1 : -1;
    capacity = len;

    data = d;
    insertHandler = inserthandler;
    removeHandler = removehandler;
  }

  Heap(unsigned long long len, int (*comparepiority)(K, K), bool maxHeap,
       void (*inserthandler)(K, V *, X *) = NULL,
       void (*removehandler)(K, V, X *) = NULL) {
    items = new Heap_Cell<K, V>[len]();
    comparePiority = comparepiority;
    expectedRoot = maxHeap ? 1 : -1;
    capacity = len;

    insertHandler = inserthandler;
    removeHandler = removehandler;
  }

  ~Heap() {}

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
        Heap_Cell<K, V> temp = items[i];
        items[i] = items[cL];
        items[cL] = temp;
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
      Heap_Cell<K, V> temp = items[i];
      items[i] = items[p];
      items[p] = temp;

      i = p;
    }
  }

  void push(K piority, V val) {
    assert(count <= capacity);
    Heap_Cell<K, V> *c = &items[count];

    c->piority = piority;
    c->value = val;

    levitate(count);

    count++;

    if (insertHandler != NULL)
      insertHandler(piority, &val, data);
  }

  V pop() {
    Heap_Cell<K, V> c = items[0];

    count--;
    items[0] = items[count];
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
    Heap_Cell<K, V> c = items[0];

    Heap_Cell<K, V> *cell = items[0];
    cell->piority = piority;
    cell->value = val;

    sink(0);

    return c.value;
  }

  V *get(K k) {
    for (unsigned long long i = 0; i < count; i++) {
      if (comparePiority(items[i].piority, k) == 0)
        return &items[i].value;
    }
    return NULL;
  }

  V remove(K k) {
    for (unsigned long long i = 0; i < count; i++) {
      if (comparePiority(items[i], k) == 0) {
        Heap_Cell<K, V> c = items[i];
        items[i] = items[--count];
        sink(i);
        if (removeHandler != NULL)
          removeHandler(k, c.value, data);
      }
    }
    return NULL;
  }

    void reset(){
        count = 0;
    }
};
