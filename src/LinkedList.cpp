#include <cassert>
#include <string>
#include <iostream>
#include <limits>

template<class V> struct Node{
    V val;
    Node<V> * next;
    Node(V d, Node<V>* next):val(d), next(next){}
    Node(V d):val(d), next(NULL) {}
};

template<class V> class LinkedListIterator{
    public:
    Node<V>* val; 

    LinkedListIterator(Node<V>* d): val(d){};

    V* next(){
        if(val == NULL) return NULL;
        Node<V>* h = val;
        val = val->next;
        return &h->val;
    }

    bool hasNext(){
       return val != NULL;
    }
};


template<typename V> class LinkedList {
    public:
    Node<V>* head = NULL;
    Node<V>* tail = NULL;
    unsigned long long count = 0;

    void shift(V d) {
        head = new Node<V>(d, head);

        if(tail == NULL){
            tail = head;
        } 

        count++;
    }

    void push(V dato) {
        Node<V>* n = new Node<V>(dato);

        if(tail != NULL) tail->next = n;

        tail = n;

        if(head == NULL)
            head = tail;

        count++;
    }


    LinkedListIterator<V>* iterate(){
        return new LinkedListIterator<V>(head);
    }
};

