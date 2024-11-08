template<class V, class R> class Iterator{
    public:
    V* val; 

    R*(*_next)(V*);
    bool(*_hasNext)(V*);

    Iterator(V* d, R*(*n)(V), bool(*hn)(V)): val(d), _next(n), _hasNext(hn) {};

    R* next(){
        return _next(val);
    }

    bool hasNext(){
        return _hasNext(val);
    }
};
