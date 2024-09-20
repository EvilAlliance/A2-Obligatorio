#include <algorithm>
#include <cassert>
#include <cstdio>
template<class K, class V> struct Cell {
    /**
     * 0 empty
     * 1 full
     * 2 to Reset
     * */
    short state;

    V value;
    K key;
    Cell(K k, V v) : key(k), value(v), state(0) {}
    Cell() : state(0) {}
};

template<class K,class V, class X> class HashTable {
public:

    void(*insertHandler)(K, V*, V* , X*);
    void(*removeHandler)(K, V, X*);
    void(*reHashHandler)(X*);
    X* data = NULL;

    unsigned long long (*fhash)(K);
    unsigned long long (*shash)(K);

    bool (*equal)(K, K);


    Cell<K, V>* items;
    unsigned long long count;
    unsigned long long len;
    unsigned long long theoricalLen;

    HashTable(unsigned long long l, bool(*e)(K, K), unsigned long long (*f)(K), unsigned long long (*s)(K), void(*inserthandler)(K, V*, V*, X*) = NULL, void(*removehandler)(K, V, X*) = NULL, void(*reHashhandler)(X*) = NULL){
        theoricalLen = l;

        len = nextPrime(l*2);
        count = 0;
        items = new Cell<K, V>[len]();

        fhash = f;
        shash = s;

        equal = e;

        insertHandler = inserthandler;
        removeHandler = removehandler;
        reHashHandler = reHashhandler;
    }

    HashTable(unsigned long long l, bool(*e)(K, K), X* d,unsigned long long (*f)(K), unsigned long long (*s)(K), void(*inserthandler)(K, V*, V*, X*) = NULL, void(*removehandler)(K, V, X*) = NULL, void(*reHashhandler)(X*) = NULL){
        theoricalLen = l;

        len = nextPrime(l*2);
        count = 0;
        items = new Cell<K, V>[len]();

        fhash = f;
        shash = s;

        data = d;
        insertHandler = inserthandler;
        removeHandler = removehandler;
        reHashHandler = reHashhandler;

        equal = e;
    }

    unsigned long long isPrime(unsigned long long n){
        if (n == 0)  return 0; 
        if (n == 1)  return 0; 
        if (n % 2 == 0) return 0; 
        for (unsigned long long i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }

    unsigned long long nextPrime(unsigned long long n){
        while(!isPrime(n)) n++;
        return n;
    }

    void reHash(){
        if(reHashHandler != NULL) reHashHandler(data);

        Cell<K, V>* oldItems = items;
        unsigned long long oldLen = len;
        unsigned long long oldCount = count;

        theoricalLen *=2;

        len = nextPrime(theoricalLen*2);
        count = 0;
        items = new Cell<K, V>[len]();

        for (unsigned long long i = 0; i < oldLen; i++) {
            Cell<K, V> c = oldItems[i];
            if(c.state == 1)
                set(c.key, c.value);
        }
    }
    
    void set(K key, V value){

        if(count*100/len > 70){
            reHash();
        }

        unsigned long long  f = fhash(key);
        unsigned long long  s = shash(key);
        for (unsigned long long i = 0; i < len; i++) {
            Cell<K, V>* c = &items[(f + s*i) % len] ;
            if(c->state == 0){
                if(insertHandler != NULL) insertHandler(key, &value, NULL, data);
                c->key = key;
                c->value = value;
                c->state = 1;
                count++;
                return;
            }else if(c->state == 1 && equal(c->key, key)){
                if(insertHandler != NULL) insertHandler(key, &value, &c->value, data);
                c->value = value;
                return;
            }else if(c->state == 2){

                for (unsigned long long i = i + 1; i < len; i++) {
                    Cell<K, V>* c2 = &items[(f + s*i) % len] ;
                    if(c2->state == 1 && equal(c2->key, key)){
                        if(insertHandler != NULL) insertHandler(key, &value, &c2->value, data);
                        c2->value = value;
                        return;
                    }else if(c2->state == 0)
                        break;
                }

                c->key = key;
                c->value = value;
                c->state = 1;
                count++;
                if(insertHandler != NULL) insertHandler(key, &value, NULL, data);
            }
        }

        reHash();
        set(key, value);
    }
    
    V* get(K key){
        unsigned long long  f = fhash(key);
        unsigned long long  s = shash(key);
        for (unsigned long long i = 0; i < len; i++) {
            Cell<K, V>* c = &items[(f + s*i) % len] ;
            if(c->state == 0)
                return NULL;
            else if(c->state == 1 && equal(c->key, key)){
                return &c->value;
            }
        }
        return NULL;
    }

    V* remove(K key){
        unsigned long long  f = fhash(key);
        unsigned long long  s = shash(key);
        for (unsigned long long i = 0; i < len; i++) {
            Cell<K, V>* c = &items[(f + s*i) % len] ;
            if(c->state == 0)
                return NULL;
            else if(c->state == 1 && equal(c->key, key)){
                c->state = 2;
                count--;
                if(removeHandler != NULL) removeHandler(key, c->value, data);
                return c->value;
            }
        }
        return NULL;
    }
};
