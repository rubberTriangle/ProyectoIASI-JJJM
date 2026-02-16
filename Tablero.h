#ifndef TABLERO_H
#define TABLERO_H
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
    void procesarMatriz(vector<vector<char>> const& chmat, vector<vector<Vehiculo*>> &m);
public:
    vector<vector<Vehiculo*>> matriz;
    Tablero();
    void importarMatriz();
    void mostrarMatriz();
};

#endif // TABLERO_H
