#ifndef TABLERO_H
#define TABLERO_H
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Tablero
{
private:
    int ancho, alto;
    string nombreTablero;
public:
    vector<vector<char>> matriz;
    Tablero();
    void importarMatriz();
    void mostrarMatriz();
};

#endif // TABLERO_H
