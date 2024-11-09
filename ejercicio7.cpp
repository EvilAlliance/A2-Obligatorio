#include <cassert>
#include <list>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Student {
    unsigned long long begin;
    unsigned long long end;
    unsigned long long total;
};

struct Result {
    Student* s;
    unsigned long long cantAlumnos;
    unsigned long long maxTotal;
    bool valid;

    /*Result* clone() {*/
    /*    Result* r = new Result();*/
    /*    r->s = new Student[cantAlumnos]();*/
    /**/
    /*    for (unsigned long long i = 0; i < cantAlumnos; i++) */
    /*        r->s[i]= {*/
    /*            .begin = s[i].begin,*/
    /*            .end = s[i].end,*/
    /*            .total = s[i].total,*/
    /*        };*/
    /**/
    /*    return r;*/
    /*}*/
};

unsigned long long consumeMax(unsigned long long* m, unsigned long long iM, unsigned long long mCount, unsigned long long avg, unsigned long long* consumed){
    unsigned long long c = 0;
    while(iM < mCount && c + m[iM] < avg){
        c += m[iM];
        iM++;
    }

    *consumed = c;

    return iM;
}

Result* _listen(unsigned long long* m, unsigned long long mCount, unsigned long long aCount, unsigned long long avg){
    Result* r = new Result();
    r->s = new Student[aCount]();

    unsigned long long iA = 0;
    unsigned long long iM = 0;
    unsigned long long extra = 0;

    while(iM < mCount && iA < aCount){
        r->s[iA] = {
            .begin = iM + 1,
            .total = 0,
        };

        iM = consumeMax(m, iM, mCount, avg, &r->s[iA].total);

        r->s[iA].end = iM;

        if(r->maxTotal < r->s[iA].total) r->maxTotal = r->s[iA].total;

        iA++;
    }

    r->cantAlumnos = iA;

    r->valid = iM == mCount;

    return r;
}

Result* listen(unsigned long long* m,unsigned long long  mCount, unsigned long long  aCount, unsigned long long avg){
    Result* base = _listen(m, mCount,  aCount, avg);

    {
        unsigned long long x = avg / 2;

        while (x > 0 && !base->valid) {
            avg += x;

            delete base;

            base = _listen(m, mCount, aCount, avg);

            x /= 2;
        }
    }

    unsigned long long diff = avg / 2;

    while (diff > 0) {
        Result* l = _listen(m, mCount, aCount, avg - diff);
        Result* r = _listen(m, mCount, aCount, avg + diff);

        if((!r->valid || (l->valid && l->maxTotal < r->maxTotal)) &&  l->maxTotal < base->maxTotal){
            delete base;
            delete r;

            base = l;
            avg -= diff;
        }else if((!l->valid || (r->valid && r->maxTotal < l->maxTotal)) &&  r->maxTotal < base->maxTotal){
            delete base;
            delete l;

            base = r;
            avg += diff;
        } 

        diff = diff / 2;
    }

    return base;
}


int main(){
    unsigned long long count;
    std::cin >> count;

    unsigned long long* m = new unsigned long long[count];
    unsigned long long total = 0;

    for (unsigned long long i = 0; i < count; i++) {
        std::cin >> m[i];
        total += m[i];
    }

    unsigned long long cantAlumnos;
    std::cin >> cantAlumnos;

    Result* r = listen(m, count, cantAlumnos, total/cantAlumnos);

    if(!r->valid) {
        std::cout << "No valido\n";
        return 1;
    }

    for (unsigned long long i = 0; i < r->cantAlumnos; i++) {
        Student s = r->s[i];
        if(s.begin == s.end && s.begin == 0) break;
        std::cout << s.total << " " << s.begin << " " << s.end << "\n";
    }


    std::cout << r->maxTotal << "\n";

    return 0;
}
