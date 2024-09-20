#include <algorithm>
#include <iostream>
#include <cassert>

template<class K,class V> struct BT_AVL {
    BT_AVL<K, V>* l;
    BT_AVL<K, V>* r;

    unsigned long long height;
    long long balance;

    V value;
    K key;

    unsigned long long getNewHeight(){
        return  1 + std::max(
            l != NULL ? l->height : 0,
            r != NULL ? r->height : 0
        );
    }

    void updateHeight(){
        height = getNewHeight();
    }

    long long getNewBalance(){
        return 
            (r != NULL ? r->height : 0) -
            (l != NULL ? l->height : 0)
        ;
    }

    void updateBalance(){
        balance = getNewBalance();
    }

    BT_AVL(K k, V v) : key(k), value(v), height(1),balance(0), l(NULL), r(NULL) {}
    BT_AVL(K k, V v, BT_AVL<K, V>le, BT_AVL<K,V>ri) : key(k), value(v), height(1), balance(0), l(le), r(ri) {}
};

template<class K,class V, class X> class AVL {
public:
    BT_AVL<K, V>* r;
    unsigned long long count;
    /**
     * -1 cunado el primero es menor
     *  0 cunado son iguales
     *  1 cuando el primero es mayor
     * */
    int(*compareKey)(K, K);
    void(*insertHandler)(K, V*, V* , X*);
    void(*removeHandler)(K, V, X*);
    X* data;

    AVL(int(*comparekey)(K, K), X* d, void(*inserthandler)(K, V*, V*, X*) = NULL, void(*removehandler)(K, V, X*) = NULL){
        compareKey = comparekey;
        insertHandler = inserthandler;
        removeHandler = removehandler;
        data = d;
    }

    AVL(int(*comparekey)(K, K), void(*inserthandler)(K, V*, V*, X*) = NULL, void(*removehandler)(K, V, X*) = NULL){
        compareKey = comparekey;
        insertHandler = inserthandler;
        removeHandler = removehandler;
    }

    ~AVL(){
    }

    void set(K key, V val){
        r = _set(r, key, val);
    }

    BT_AVL<K, V>* balanceNode(BT_AVL<K, V>* root){
        if(root == NULL) return NULL;
        if(root->balance == 2 && root->r->balance >= 0)
            return rotateLeft(root);
        else if(root->balance == -2 && root->l->balance <= 0)
            return rotateRight(root);
        else if(root->balance == 2 && root->r->balance < 0)
            return rotateRightLeft(root);
        else if(root->balance == -2 && root->l->balance > 0)
            return rotateLeftRight(root);

        return root;
    }

    BT_AVL<K, V>* _set(BT_AVL<K, V>* root, K key, V val){
        if(root == NULL){
            count++;
            if(insertHandler != NULL) insertHandler(key, &val, NULL, data);
            return new BT_AVL<K, V>(key, val);
        } 
        int cond = compareKey(root->key, key);
        if(cond == 0){
            if(insertHandler != NULL) insertHandler(key, &val, &root->value, data);
            root->value = val;
            return root;
        }else if(cond == -1){
            root->r = _set(root->r, key, val);
        }else if(cond == 1){
            root->l = _set(root->l, key, val);
        }


        root->updateHeight();
        root->updateBalance();

        return balanceNode(root);
    }


    bool verifyTree(BT_AVL<K, V>* root){
        if(root == NULL) return 1;
        if(!verifyTree(root->l)) return 0;
        if(!verifyTree(root->r)) return 0;

        unsigned long long height = root->getNewHeight();

        long long balance = root->getNewBalance();

        if(balance != root->balance){
            std::cout << "Wrong Balance\n" ;
            _mostrar(root, 0);
            return 0;
        }

        if(height != root->height){
            std::cout << "Wrong Height\n";
            _mostrar(root, 0);
            return 0;
        }

        if(root->l != NULL && compareKey(root->key, root->l->key) != 1){
            std::cout << "Left child has larger key than the parenet\n";
            _mostrar(root, 0);
            return 0;
        }


        if(root->r != NULL && compareKey(root->key,root->r->key) != -1){
            std::cout << "Right child has larger key than the parenet\n";
            _mostrar(root, 0);
            return 0;
        }

        return 1;
    }

   BT_AVL<K,V>* rotateLeft(BT_AVL<K,V>* root){
        assert(root != NULL);
        BT_AVL<K, V>* a = root;
        BT_AVL<K, V>* b = a->r;

        BT_AVL<K, V>* II = b->l;

        b->l = a;
        a->r = II;

        if (b->balance == 0){
            a->balance = 1;
            b->balance = -1;

            b->height += 1;
        } else {
            a->balance = 0;
            b->balance = 0;
        }

        a->height = 1 + (II != NULL ? II->height : 0);

        return b;
    }

    BT_AVL<K,V>* rotateRight(BT_AVL<K,V>* root){
        assert(root != NULL);
        BT_AVL<K, V>* a = root;
        BT_AVL<K, V>* b = a->l;

        BT_AVL<K, V>* II = b->r;

        b->r = a;
        a->l = II;

        if (b->balance == 0){
            a->balance = -1;
            b->balance = 1;

            b->height += 1;
        } else {
            a->balance = 0;
            b->balance = 0;
        }

        a->height = 1 + (II != NULL ? II->height : 0);

        return b;
    }


    BT_AVL<K,V>*  rotateRightLeft(BT_AVL<K,V>* root){
        assert(root != NULL);

        BT_AVL<K,V>* a = root;
        BT_AVL<K,V>* b = a->r;
        BT_AVL<K,V>* c = b->l;

        BT_AVL<K, V>* III = c->r;
        BT_AVL<K, V>* II = c->l;

        b->l = III;
        c->r = b;
        a->r = II;
        c->l = a;

        if(c->balance == 0){
            a->balance = 0;
            b->balance = 0;
        }else if(c->balance > 0){
            a->balance = -1;
            b->balance = 0;
        }else {
            a->balance = 0;
            b->balance = 1;
        }

        a->height = c->height;

        int temp = c->height;
        c->height = b->height;
        b->height = temp;

        c->balance = 0;

        return c;
    }

    BT_AVL<K,V>* rotateLeftRight(BT_AVL<K,V>* root){
        assert(root != NULL);

        BT_AVL<K,V>* a = root;
        BT_AVL<K,V>* b = a->l;
        BT_AVL<K,V>* c = b->r;

        BT_AVL<K, V>* III = c->l;
        BT_AVL<K, V>* II = c->r;

        b->r = III;
        c->l = b;
        a->l = II;
        c->r = a;

        if(c->balance == 0){
            a->balance = 0;
            b->balance = 0;
        }else if(c->balance < 0){
            a->balance = 1;
            b->balance = 0;
        }else {
            a->balance = 0;
            b->balance = -1;
        }

        a->height = c->height;

        int temp = c->height;
        c->height = b->height;
        b->height = temp;

        c->balance = 0;

        return c;
    }

    V* get(K key){
        if(r == NULL) return NULL;

        BT_AVL<K, V>* root = r;

        int cond = 0;
        while(root != NULL && (cond = compareKey(root->key, key)) != 0)
            root = cond == -1 ? root->r : root->l;

        if(root == NULL) return NULL;
        return &root->value;
    }
    int c = 0;

    V _remove(BT_AVL<K, V>** root, K key){
        assert(root != NULL);

        V ret;

        BT_AVL<K, V>* f = *root;

        if(f == NULL) return 0;
        int cond = compareKey(f->key, key);
        if(cond == -1){
            ret = _remove(&f->r, key);
        } else        if(cond == 1) {
            ret = _remove(&f->l, key);
        }else if(cond == 0){
            if (f->r == NULL && f->l == NULL){
                ret = f->value;

                delete f;
                *root = NULL;

                count--;
                return ret;
            } else if(f->r == NULL || f->l == NULL){
                BT_AVL<K, V>* toDel = f;

                ret = f->value;
                *root = f->l != NULL ? f->l : f->r;

                delete toDel;
                toDel = NULL;

                f = *root;
                count--;
            } else {
                BT_AVL<K, V>* minMax = f->l;

                while(minMax->r != NULL)
                    minMax = minMax->r;

                f->value = minMax->value;
                f->key = minMax->key;

                _remove(&f->l, minMax->key);
            }
        }

        f->updateHeight();
        f->updateBalance();

        *root = balanceNode(f);

       return ret;
    }

    V remove(K key){
        V ret =  _remove(&r, key);

        if(removeHandler != NULL) removeHandler(key, ret, data);

        return ret;
    }

    void mostrar(int indent = 0){
        _mostrar(r, 0);
    }

    void _mostrar(BT_AVL<K, V>* r, int depth){
        for(unsigned long long i = 0; i < depth; i++)
            std::cout << "|  ";

        if(r == NULL) {
            std::cout << "ͱ——— NULL" << '\n';

        }else{
            std::cout << "ͱ——— Key: " << r->key << ", Value: " << r->value << ", Balance: " << r->balance << ", Height: " << r->height << '\n';

            _mostrar(r->r, depth + 1); 
            _mostrar(r->l, depth + 1); 
        }

    }
};
