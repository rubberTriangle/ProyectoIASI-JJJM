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
    
    //PRE: chmat debe tener al menos 1 elemento
    void procesarMatriz(vector<vector<char>> const& chmat, vector<vector<Vehiculo*>> &m);
public:
    vector<vector<Vehiculo*>> matriz;
    Tablero();
    void importarMatriz();
    void mostrarMatriz();
    
    ~Tablero();
};

#endif // TABLERO_H
