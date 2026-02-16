#include <iostream>

#include "Tablero.h"

using namespace std;

int main()
{
    Tablero tablero;
    tablero.mostrarMatriz();
    cout << endl;
    vector<pair<Vehiculo, Tablero::direccion>> movimientosPosibles;

    movimientosPosibles = tablero.movimientosPosibles();
    Tablero auxiliar;

    for (int i = 0; i < movimientosPosibles.size(); i++) {
        auxiliar = tablero;
        auxiliar.realizarMovimiento(movimientosPosibles[i]);
        auxiliar.mostrarMatriz();
        cout << endl;
    }

    return 0;
}
