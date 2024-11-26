#include <algorithm>
#include <cassert>
#include <random>
#include <string>
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
    unsigned long long a;
    unsigned long long b;

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

Result lessDist(City* c, Result a, Result b){
    if(a.dist < b.dist) return a;
    if(a.dist > b.dist) return b;
    if(c[a.a].p + c[a.b].p > c[b.a].p + c[b.b].p) return a;
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

void searchBetterResult(City* c, Result* r, unsigned long long low, unsigned long long high){
    for (unsigned long long i = low; i <= high; i++) {
        Result a {
            .a = r->a,
            .b = i,
            .dist =dist(c[r->a], c[i]),
       };

       Result b {
           .a = i,
           .b = r->b,
           .dist = dist(c[r->b], c[i]),
       };

        *r = lessDist(c, a, lessDist(c, b, *r));
    }
}

Result searchNearest(City* c, unsigned long long  index, unsigned long long low, unsigned long long high){
    Result r = {
        .a = index,
        .b = low,
        .dist = dist(c[index], c[low])
    };

    for (unsigned long long i = low + 1; i <= high; i++) {
        double d = dist(c[index], c[i]);

        Result a {
            .a = index,
            .b = i,
            .dist = d
        };

        r = lessDist(c, r, a);
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
    if(low == high)
        return {
            .a = low,
            .invalid = false,
            .partial = true
        };

    if(high - low == 1) 
        return {
            .a = low,
            .b = high,

            .dist = dist(c[low], c[high]),

            .invalid = false,
            .partial = false,
        };

    unsigned long long mid = low + ((high - low) / 2);

    Result a = _divideConquer(c, low, mid, count);
    Result b = _divideConquer(c, mid + 1, high, count);

    if(a.partial)
       a = searchNearest(c, a.a, mid+1 , high);
    /*else*/
    /*    searchBetterResult(c, &a, mid + 1, high);*/

    if(b.partial)
        b = searchNearest(c, b.a, low, mid);
    /*else*/
    /*    searchBetterResult(c, &b, low, mid);*/

    Result r = lessDist(c, a, b);

    /*double realDist = dist(c[r.a], c[r.b]);*/
    unsigned long long realDist = max(abs(c[r.a].coord.y - c[r.b].coord.y), abs(c[r.a].coord.x - c[r.b].coord.x));
    /*unsigned long long realDist = max(abs(c[r.a].coord.y - c[r.b].coord.y), (long long) 1);*/

    unsigned long long newL;
    unsigned long long newH;
    adjustRange(c, low, mid, high, realDist, &newL, &newH);

    /*for(unsigned long long i = newL; i < newH; i++){*/
    /*    for (unsigned long long j = newL; j < newH; j++) {*/
    /*        if(i == j) continue;*/
    /**/
    /*        Result temp = {*/
    /*            .a = i,*/
    /*            .b = j,*/
    /*            .dist = dist(c[i], c[j]),*/
    /*        };*/
    /**/
    /*        r = lessDist(c, r, temp);*/
    /*    }*/
    /*}*/

    if(newL != low || newH != high)
        r = lessDist(c, r ,_divideConquer(c, newL, newH, count));
    else{
        for(unsigned long long i = newL; i < newH; i++){
            for (unsigned long long j = i + 1; j < newH; j++) {
                if(abs(c[i].coord.y - c[j].coord.y) > realDist) break;

                unsigned long long newRealDist = max(abs(c[i].coord.y - c[j].coord.y), abs(c[i].coord.x - c[j].coord.x));

                if(newRealDist <= realDist){
                    Result temp = {
                        .a = i,
                        .b = j,
                        .dist = dist(c[i], c[j]),
                    };

                    if(temp.dist <= r.dist)
                        r = lessDist(c, r, temp);
                        

                   if(newRealDist < realDist){
                        realDist = newRealDist;
                        adjustRange(c, low, mid, high, newRealDist, &newL, &newH);
                        if(newL != low || newH != high)
                            goto afterFor;
                    }
                }
            }
        }

    afterFor:
        if(newL == low && newH == high) return r;

        r = lessDist(c, r ,_divideConquer(c, newL, newH, count));
    }

    /*else {*/
        /*std::cout << "newL = "  << newL << " && newH = " << newH << " && diff = " << newH - newL << "\n";*/
    /*    for(unsigned long long i = newL; i < newH; i++){*/
    /*        for (unsigned long long j = i + 1; j < newH; j++) {*/
    /*            if(abs(c[i].coord.y - c[j].coord.y) > realDist) break;*/
    /*            if(i == j) continue;*/
    /**/
    /*            Result temp = {*/
    /*                .a = i,*/
    /*                .b = j,*/
    /*                .dist = dist(c[i], c[j]),*/
    /*            };*/
    /**/
    /*            r = lessDist(c, r, temp);*/
    /*        }*/
    /*    }*/
    /*}*/
    return r;
}

Result divideConquer(City* c, unsigned long long count){
    return _divideConquer(c, 0, count - 1, count);
}

void printCity(City a){
    std::cout << a.coord.x << " " << a.coord.y << " " << a.p << '\n';
}

void print(City* c, Result r){
    City a = c[r.a];
    City b = c[r.b];

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
