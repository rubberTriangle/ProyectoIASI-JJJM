#ifndef TABLERO_H
#define TABLERO_H
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include "vehiculo.h"

using namespace std;

class Tablero {
private:
    int ancho, alto;
    string nombreTablero;
    std::list<Vehiculo*> vehiculos;
    pair<int, int> posX;
    pair<int, int> posZ;
    
    //PRE: chmat debe tener al menos 1 elemento
    void procesarMatriz(vector<vector<char>> const& chmat, vector<vector<Vehiculo*>> &m);
    void importarMatriz();


public:
    enum direccion{IZQUIERDA, ARRIBA, DERECHA, ABAJO};
    vector<vector<Vehiculo*>> matriz;
    
    Tablero();
    Tablero(Tablero const& other);
    void mostrarMatriz();
    
    string key();//genera una string única al estado actual de la matriz
    
    vector<pair<Vehiculo*, direccion> > movimientosPosibles(); // 0 izq, 1, arr, 2 der, 3 aba
    void realizarMovimiento(pair<Vehiculo*, direccion> v);
    bool comprobarFin();
    int calcHeuristicaSimple() const;
    int calcHeuristicaVehiculos() const;
    bool operator==(const Tablero& otro) const {
        if (this->matriz.size() != otro.matriz.size() ||
            this->matriz[0].size() != otro.matriz[0].size()) {
            return false;
        }

        for (size_t i = 0; i < this->matriz.size(); i++) {
            for (size_t j = 0; j < this->matriz[i].size(); j++) {

                Vehiculo* v1 = this->matriz[i][j];
                Vehiculo* v2 = otro.matriz[i][j];

                if (v1 == nullptr && v2 == nullptr) {
                    continue;
                }

                if (v1 == nullptr || v2 == nullptr) {
                    return false;
                }

                if (v1->letra != v2->letra) {
                    return false;
                }
            }
        }

        return true;
    }

    ~Tablero();
};

#endif // TABLERO_H
