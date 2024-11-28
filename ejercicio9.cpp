#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Player{
    int ritmo;
    int tiro;
    int pase;
    int regate;
    int defensa;
    int fisico;

    int formaFisica;
    int confianza;

    int salario;

    bool extranjero;

    bool lesionado = false;
};

int calcularValoracion(Player p){
    if(p.lesionado) return 0;

    int val = p.ritmo + p.tiro + p.pase + p.regate + p.defensa + p.fisico;
    val /= 6;

    val += p.formaFisica + p.confianza;

    return val;
}

int calcularOptimaFormacion(Player* p, int J, int P, int E){
    int**** tab = new int***[J]();
    for (int i = 0; i < J; i++) {
        tab[i] = new int**[P + 1]();
        for(int j = 0; j <= P; j++){
            tab[i][j] = new int*[E + 1]();
            for(int k = 0; k <= E; k++){
                tab[i][j][k] = new int[12]();
            }
        }
    }

    int valuacion0 = calcularValoracion(p[0]);

    for(int j = 0; j <= P; j++){
        for(int k = 0; k <= E; k++){
            for (int o = 1; o <= 11; o++){
                if(j < p[0].salario) tab[0][j][k][o] = 0;
                else if(k == 0 && p[0].extranjero) tab[0][j][k][o] = 0;
                else tab[0][j][k][o] = valuacion0;
            }
        }
    }

    // i es el jugador
    // j es el presupuesto diponible
    // k cantidad de de extrngeros
    // o el jugudor principal
    for(int i = 1; i < J; i++){
        int valuacion = calcularValoracion(p[i]);
        for(int j = 0; j <= P; j++){
            for(int k = 0; k <= E; k++){
                for (int o = 1; o <= 11; o++){
                    if(k == 0 && p[i].extranjero) tab[i][j][k][o] = tab[i - 1][j][k][o];
                    else if(j < p[i].salario) tab[i][j][k][o] = tab[i - 1][j][k][o];
                    else tab[i][j][k][o] = max(tab[i - 1][j][k][o], tab[i - 1][j - p[i].salario][k - p[i].extranjero][o - 1] + valuacion);
              }
            }
        }
    }

    return tab[J - 1][P][E][11] / 11;
}

int main(){
    int J;
    std::cin >> J;

    Player* p = new Player[J];

    for(int i = 0; i < J; i++){
        std::cin >> p[i].ritmo;
        std::cin >> p[i].tiro;
        std::cin >> p[i].pase;
        std::cin >> p[i].regate;
        std::cin >> p[i].defensa;
        std::cin >> p[i].fisico;

        string formaFisica;
        std::cin >> formaFisica;

        if(formaFisica.compare("Lesionado") == 0){
            p[i].lesionado = true;
        }else if(formaFisica.compare("Mala") == 0){
            p[i].formaFisica = -5;
        } else if(formaFisica.compare("Normal") == 0){
            p[i].formaFisica = 0;
        }else if(formaFisica.compare("Buena") == 0){
            p[i].formaFisica = 5;
        } else {
            assert(false && "unreacable");
        }


        std::cin >> p[i].salario;

        string extrangero;
        std::cin >> extrangero;

        if(extrangero.compare("Si") == 0){
            p[i].extranjero = true;
        }else if(extrangero.compare("No") == 0){
            p[i].extranjero = false;
        } else {
            assert(false && "unreacable");
        }

        string confianza;
        std::cin >> confianza;

        if(confianza.compare("Baja") == 0){
            p[i].confianza = -5;
        } else if(confianza.compare("Regular") == 0){
            p[i].confianza = 0;
        }else if(confianza.compare("Alta") == 0){
            p[i].confianza = 5;
        } else {
            assert(false && "unreacable");
        }
    }

    int P;
    std::cin >> P;

    int E;
    std::cin >> E;

    std::cout << calcularOptimaFormacion(p, J, P, E) << "\n";

    return 0;
}
