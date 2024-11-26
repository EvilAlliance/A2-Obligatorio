#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std;

struct Point{
    long long x;
    long long y;
};

struct City{
    Point coord;
    long long p;
};

struct Result{
    City a;
    City b;

    double dist;

    bool invalid;
    bool partial;
};

double root(unsigned long long n, unsigned long long iteration){
    assert(n <= std::numeric_limits<double>::max());

    double x = (double) n;

    for(unsigned long long i = 0; i < iteration; i++)
        x -= (x*x - (double)n)/ (2*x);


    return x;
}

/*double dist(City a, City b){*/
/*    unsigned long long dx = b.coord.x - a.coord.x;*/
/*    unsigned long long dy = b.coord.y - a.coord.y;*/
/**/
/*    double d = root(dx*dx + dy*dy, 1000) + (double)abs(b.p - a.p)/(double)max(b.p, a.p);*/
/**/
/*    return d;*/
/*}*/

double dist(City a, City b){
    unsigned long long dx = b.coord.x - a.coord.x;
    unsigned long long dy = b.coord.y - a.coord.y;

    double d = root(dx*dx + dy*dy, 1000) + (double)abs(b.p - a.p)/(double)max(b.p, a.p);

    return d;
}

double dist2(City a, City b){
    unsigned long long dx = b.coord.x - a.coord.x;
    unsigned long long dy = b.coord.y - a.coord.y;

    double d = root(dx*dx + dy*dy, 20);

    return d;
}

Result lessDist(Result a, Result b){
    if(a.dist < b.dist) return a;
    if(a.dist > b.dist) return b;
    if(a.a.p + a.b.p > b.a.p + b.b.p) return a;
    return b;
}

void  _quicksortY(City* c, unsigned long long low, unsigned long long high, unsigned long long count){
    if(low >= high || high >= count || low < 0) return;

    City pivot = c[high];
    unsigned long long j = low;

    for (unsigned long long i  = low; i <= high - 1; i++) {
        if(c[i].coord.y <= pivot.coord.y){
            City temp = c[i];
            c[i] = c[j];
            c[j] = temp;

            j++;
        }
    }

    City temp = c[j];
    c[j] = c[high];
    c[high] = temp;

    _quicksortY(c, low, j - 1, count);
    _quicksortY(c, j + 1, high, count);
}

void quicksortY(City* c, unsigned long long count){
    _quicksortY(c, 0, count - 1, count);
}

void  _quicksortX(City* c, unsigned long long low, unsigned long long high, unsigned long long count){
    if(low >= high || high >= count || low < 0) return;

    City pivot = c[high];
    unsigned long long j = low;

    for (unsigned long long i  = low; i <= high - 1; i++) {
        if(c[i].coord.x <= pivot.coord.x){
            City temp = c[i];
            c[i] = c[j];
            c[j] = temp;

            j++;
        }
    }

    City temp = c[j];
    c[j] = c[high];
    c[high] = temp;

    _quicksortX(c, low, j - 1, count);
    _quicksortX(c, j + 1, high, count);
}

void quicksortX(City* c, unsigned long long count){
    _quicksortX(c, 0, count - 1, count);
}

Result searchNearest(City* c, City  index, unsigned long long low, unsigned long long high){
    Result r = {
        .a = index,
        .b = c[low],
        .dist = dist(index, c[low])
    };

    for (unsigned long long i = low + 1; i <= high; i++) {
        double d = dist(index, c[i]);

        Result a {
            .a = index,
            .b = c[i],
            .dist = d
        };

        r = lessDist(r, a);
    }

    return r;
}

void adjustRange(City* c, unsigned long long low, unsigned long long mid, unsigned long long high, unsigned long long realDist, unsigned long long* newLow, unsigned long long* newHigh){
    unsigned long long newL = low;
    /*unsigned long long newL = max(a.a, a.b);*/
    while(c[mid].coord.y - c[newL].coord.y > realDist) newL++;
    /*while(dist2(c[newL], c[mid]) > realDist) newL++;*/

    unsigned long long newH = high;
    /*unsigned long long newH = min(b.a, b.b);*/
    while(c[newH].coord.y - c[mid].coord.y > realDist) newH--;
    /*while(dist2(c[newH], c[mid]) > realDist) newH--;*/

    *newLow = newL;
    *newHigh = newH;
}

Result _divideConquer(City* c, unsigned long long low, unsigned long long high, unsigned long long count){
    if(high - low == 1) 
        return {
            .a = c[low],
            .b = c[high],

            .dist = dist(c[low], c[high]),

            .invalid = false,
            .partial = false,
        };

    if(high - low == 2) 
        return lessDist({
            .a = c[low],
            .b = c[low + 1],

            .dist = dist(c[low], c[low + 1]),

            .invalid = false,
            .partial = false,
        },{
            .a = c[low + 1],
            .b = c[high],

            .dist = dist(c[low + 1], c[high]),

            .invalid = false,
            .partial = false,
        });

    unsigned long long mid = low + ((high - low) / 2);

    Result a = _divideConquer(c, low, mid, count);
    Result b = _divideConquer(c, mid + 1, high, count);

    if(a.partial)
       a = searchNearest(c, a.a, mid+1 , high);

    if(b.partial)
        b = searchNearest(c, b.a, low, mid);

    Result r = lessDist(a, b);

    unsigned long long realDist = r.dist;

    unsigned long long length = (high - low) + 1;
    unsigned long long realIndex = 0;
    City* cx = new City[length]();

    for (unsigned long long i = 0; i <= length; i++) {
        if(abs(c[mid].coord.x - c[low + i].coord.x) > realDist) continue;

        cx[realIndex] = c[low + i];

        realIndex++;
    }

    quicksortX(cx, realIndex);


    for(unsigned long long i = 0; i <= realIndex; i++){
        for (unsigned long long j = i + 1; j <= realIndex; j++) {
            if(abs(c[i].coord.x - c[j].coord.x) > realDist) break;
            if(i == j) continue;

            Result temp = {
                .a = c[i],
                .b = c[j],
                .dist = dist(c[i], c[j]),
            };

            r = lessDist(r, temp);
        }
    }

    return r;
}

Result divideConquer(City* c, unsigned long long count){
    return _divideConquer(c, 0, count - 1, count);
}

void printCity(City a){
    std::cout << a.coord.x << " " << a.coord.y << " " << a.p << '\n';
}

void print(City* c, Result r){
    City a = r.a;
    City b = r.b;

    if(a.coord.x < b.coord.x) {
        printCity(a);
        printCity(b);
    }else if (a.coord.x == b.coord.x){
        if(a.coord.y < b.coord.y) {
            printCity(a);
            printCity(b);
        } else {
            printCity(b);
            printCity(a);
        }
    } else {
        printCity(b);
        printCity(a);
    }
}

int main()
{
    unsigned long long count;
    std::cin >> count;

    City* c = new City[count];
    for (unsigned long long i = 0; i < count; i++) {
        std::cin >> c[i].coord.x;
        std::cin >> c[i].coord.y;
        std::cin >> c[i].p;
    }

    quicksortY(c, count);

    Result r = divideConquer(c, count);

    print(c, r);

    return 0;
}
