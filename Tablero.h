#ifndef TABLERO_H
#define TABLERO_H
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include "vehiculo.h"

using namespace std;

class Tablero
{
private:
    int ancho, alto;
    string nombreTablero;
    std::list<Vehiculo*> vehiculos;
    pair<int, int> posX;
    
    //PRE: chmat debe tener al menos 1 elemento
    void procesarMatriz(vector<vector<char>> const& chmat, vector<vector<Vehiculo*>> &m);
    void importarMatriz();
public:
    vector<vector<Vehiculo*>> matriz;
    Tablero();
    void mostrarMatriz();
    vector<pair<Vehiculo, int>> movimientosPosibles(); // 0 izq, 1, arr, 2 der, 3 aba
    void realizarMovimiento(pair<Vehiculo, int> v);
    
    ~Tablero();
};

#endif // TABLERO_H
