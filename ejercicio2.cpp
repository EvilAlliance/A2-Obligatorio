#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "./src/HashTable.cpp"

using namespace std;

struct Libery {
    unsigned long long enable;
    unsigned long long disable;
};

struct Book {
    string name;
    bool enable;
};

bool equal(int a, int b){
    return a == b;
}

void insertHandler(int key, Book* value, Book* prev, Libery* data){
    if(prev == NULL){
        if(value->enable) data->enable++; else data->disable++;
    }else if(!prev->enable){
        data->enable++;
        data->disable--;
    }
}

void reHashHandler(Libery* data){
    data->enable = 0;
    data->disable = 0;
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

int main(){
    Libery data = {0};

    unsigned long long quant = 0;
    
    std::cin >> quant;
    HashTable<int,Book, Libery>* p = new HashTable<int,Book, Libery>(quant, equal, &data, hash32shift,hash32shiftmult, insertHandler, NULL, reHashHandler);

    for(unsigned long long i = 0; i < quant; i++){
        string op;
        std::cin >> op;
        if(op.compare("ADD") == 0){
            Book b = {
                .enable = 1,
            };
            int key;
            std::cin >> key;
            std::cin >> b.name;
            unsigned long long oldCount  = p->count;

            Book* b1 = p->get(key);

            p->set(key, b);
            if(b1 == NULL && oldCount == p->count){
                std::cout << "[Error] Added incorrectclid oldcount and count are the same when the key does not exist";
            }else if (b1 != NULL  && oldCount + 1 == p->count) {
                std::cout << "[Error] Added incorrectclid oldcount + 1 and count are the same when the key does exist";
            }
        }else if(op.compare("FIND") == 0){
            int key;
            std::cin >> key;

            Book* b = p->get(key);
            if(b == NULL){
               std::cout << "libro_no_encontrado\n";
            }else{
              std::cout << b->name << " " << (b->enable ? "H" : "D") << "\n";
            }

        }else if(op.compare("TOGGLE") == 0){
            int key;
            std::cin >> key;
            Book* b = p->get(key);
            if(b == NULL){
             std::cout << "libro_no_encontrado\n";
            } else {
                if(b->enable){
                    data.enable--;
                    data.disable++;
                    b->enable = 0;
                } else {
                    data.enable++;
                    data.disable--;
                    b->enable = 1;
                }
            }
        }else if(op.compare("COUNT") == 0){
            std::cout << p->count << " " << data.enable << " " << data.disable << "\n";
        }
    }

    return 0;
}
