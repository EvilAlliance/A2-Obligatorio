#include "./src/AVL.cpp"
#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int compare(int a, int b){
    if(a == b) return 0;
    if(a < b) return -1;
    else return 1;
}

struct Libery {
    unsigned long long enable;
    unsigned long long disable;
};

struct Book {
    string name;
    bool enable;
};

void insertHandler(int key, Book* value, Book* prev, Libery* data){
    if(prev == NULL){
        data->enable++;
        return;
    }else if(!prev->enable){
        data->enable++;
        data->disable--;
    }
}


int main(){
    Libery data = {0};
    AVL<int,Book, Libery>* p = new AVL<int,Book, Libery>(compare, &data, insertHandler);

    unsigned long long quant = 0;
    
    std::cin >> quant;

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

            p->set(key, b);
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
