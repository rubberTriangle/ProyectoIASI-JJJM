#include <iostream>

#include "Tablero.h"

using namespace std;

int main()
{
    Tablero tablero;
    cout << "Matriz cargada:\n";
    tablero.mostrarMatriz();
    cout << endl;
    vector<pair<Vehiculo*, Tablero::direccion>> movimientosPosibles;

    movimientosPosibles = tablero.movimientosPosibles();
    Tablero* auxiliar;

    for (int i = 0; i < movimientosPosibles.size(); i++) {
        cout << "Posible movimiento " << i+1 << ':' << endl;
        auxiliar = new Tablero(tablero);
        auxiliar->realizarMovimiento(movimientosPosibles[i]);
        auxiliar->mostrarMatriz();
        delete auxiliar;
        cout << endl;
    }

    return 0;
}
