#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Point  {
    unsigned long long x;
    unsigned long long y;
};

struct Options {
    unsigned long long op;
    Point p;
};

void  _quicksortO(Options* c, unsigned long long low, unsigned long long high, unsigned long long count){
    if(low >= high || high >= count || low < 0) return;

    Options pivot = c[high];
    unsigned long long j = low;

    for (unsigned long long i  = low; i <= high - 1; i++) {
        if(c[i].op <= pivot.op){
            Options temp = c[i];
            c[i] = c[j];
            c[j] = temp;

            j++;
        }
    }

    Options temp = c[j];
    c[j] = c[high];
    c[high] = temp;

    _quicksortO(c, low, j - 1, count);
    _quicksortO(c, j + 1, high, count);
}

void quicksortO(Options* c, unsigned long long count){
    _quicksortO(c, 0, count - 1, count);
}

void  _quicksortF(long long* c, unsigned long long low, unsigned long long high, unsigned long long count){
    if(low >= high || high >= count || low < 0) return;

    long long pivot = c[high];
    unsigned long long j = low;

    for (unsigned long long i  = low; i <= high - 1; i++) {
        if(c[i] > pivot){
            long long temp = c[i];
            c[i] = c[j];
            c[j] = temp;

            j++;
        }
    }

    long long temp = c[j];
    c[j] = c[high];
    c[high] = temp;

    _quicksortF(c, low, j - 1, count);
    _quicksortF(c, j + 1, high, count);
}

void quicksortF(long long* c, unsigned long long count){
    _quicksortF(c, 0, count - 1, count);
}

int vx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
int vy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

bool validPosition(unsigned long long x, unsigned long long y, unsigned long long n){
    return 0 <= x && x < n && 0 <= y && y < n;
}

bool canPlantFlower(long long** m, unsigned long long x, unsigned long long y, unsigned long long n, long long* f, unsigned long long fI){
    for (unsigned long long i = 0; i < 8; i++) {
        unsigned long long newX = x + vx[i];
        unsigned long long newY = y + vy[i];
        if(!validPosition(newX, newY, n) || m[newX][newY] == -1) continue;
        if(m[newX][newY] == fI) return false;
    }

    return f[fI] == -1 || f[fI] == y;
}

void markPlant(unsigned long long*** cantPlant, unsigned long long x, unsigned long long y, unsigned long long n, unsigned long long fI) {
    for (unsigned long long i = 0; i < 8; i++) {
        unsigned long long newX = x + vx[i];
        unsigned long long newY = y + vy[i];
        if(!validPosition(newX, newY, n)) continue;
        cantPlant[newX][newY][fI]++;
    }
}

void unmarkPlant(unsigned long long*** cantPlant, unsigned long long x, unsigned long long y, unsigned long long n, unsigned long long fI) {
    for (unsigned long long i = 0; i < 8; i++) {
        unsigned long long newX = x + vx[i];
        unsigned long long newY = y + vy[i];
        if(!validPosition(newX, newY, n)) continue;
        cantPlant[newX][newY][fI]--;
    }
}

void _BT(unsigned long long*** cantPlant, Options* ops, unsigned long long I, long long** m, unsigned long long n, long long* f, unsigned long long countFloresRes , unsigned long long countFlores,unsigned long long* plantadosMax, unsigned long long plantados){
    if(plantados + ((n*n) - I) < *plantadosMax) return;

    for(unsigned long long i = I; i < n*n; i++) {
        Options op = ops[i];
        Point p = op.p;
        unsigned long long x = p.x;
        unsigned long long y = p.y;

        for (unsigned long long fI = 0; fI < countFloresRes; fI++) {
            if(cantPlant[x][y][fI] > 0) continue;

            m[x][y] = fI;
            markPlant(cantPlant, x, y, n, fI);

            _BT(cantPlant, ops, i + 1, m, n, f, countFloresRes, countFlores, plantadosMax,plantados + 1);

            unmarkPlant(cantPlant, x, y, n, fI);
            m[x][y] = -1;
        }

        for (unsigned long long fI = countFloresRes; fI < countFlores; fI++) {
            if(cantPlant[x][y][fI] > 0) continue;

            m[x][y] = fI;
            markPlant(cantPlant, x, y, n, fI);

            _BT(cantPlant, ops, i + 1, m, n, f, countFloresRes, countFlores, plantadosMax,plantados + 1);

            unmarkPlant(cantPlant, x, y, n, fI);
            m[x][y] = -1;
        }
    }

    *plantadosMax = max(*plantadosMax, plantados);
}

unsigned long long countOptions(unsigned long long x, unsigned long long y, long long* f,  unsigned long long fc){
    unsigned long long op = 0;

    for (unsigned long long i = 0; i < fc; i++) {
        if(f[i] != -1 && f[i] != y) continue;
        op++;
    }

    return op;
}

void markFlowerRes(unsigned long long*** c, unsigned long long x, unsigned long long y, long long* f, unsigned long long cf){
    for (unsigned long long i = 0; i < cf; i++) {
        if(f[i] == -1 || f[i] == y) continue;
        c[x][y][i]++;
    }
}

unsigned long long BT(long long** m, unsigned long long n, long long* f, unsigned long long cfRes,unsigned long long cf){
    Options* ops = new Options[n * n]();
    unsigned long long*** cantPlant = new unsigned long long**[n];
    for(unsigned long long x = 0; x < n; x++){
        cantPlant[x] = new unsigned long long*[n];

        for (unsigned long long y = 0; y < n; y++) {
            cantPlant[x][y] = new unsigned long long[cf]();
            markFlowerRes(cantPlant, x, y, f, cf);

            ops[y*n + x] = {
                .op = countOptions(x, y, f, cf),
                .p = {
                    .x = x,
                    .y = y,
                },
            };
        }
    }

    quicksortO(ops, n*n);

    unsigned long long max = 0;
    _BT(cantPlant, ops, 0,m, n, f, cfRes ,cf, &max, 0);
    return  max;
}

int main() {
    long long n;
    std::cin >> n;

    // Mat coord seran [x][y]
    long long** mat = new long long*[n];

    for (unsigned long long i = 0; i < n; i++) {
        mat[i] = new long long[n];

        for(unsigned long long j = 0; j < n; j ++)
            mat[i][j] = -1;
    }

    unsigned long long cantFlores;

    std::cin >> cantFlores;

    long long* f = new long long[cantFlores];

    for(unsigned long long i = 0; i < cantFlores; i++){
        string p;
        std::cin >> p;
        std::cin >> f[i];
    }

    quicksortF(f, cantFlores);

    unsigned long long i = 0;
    while(i < cantFlores && f[i] != -1) i++;

    unsigned long long max = BT(mat, n, f, i, cantFlores);

    std::cout << max << "\n";

    return 0;
}
