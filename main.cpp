#include <iostream>
#include <climits>

#include "Tablero.h"

using namespace std;

int main()
{
    string heuristicas[2] = {"Distancia Manhattan simple", "Distancia Manhattan por vehículos"};
    string heuristicaSeleccionada;

    cout << "Elige la heurística a utilizar: (Presiona Enter para valor predeterminado)" << endl;
    for(int i = 0; i < 2; i++){
        cout << "[" << i << "] " << heuristicas[i] << endl;
    }

    getline(std::cin, heuristicaSeleccionada);

    if (heuristicaSeleccionada.empty()) {
        heuristicaSeleccionada = "0";
    }

    Tablero tablero;
    cout << "Matriz cargada:\n";
    tablero.mostrarMatriz();
    cout << endl;
    vector<pair<Vehiculo*, Tablero::direccion>> movimientosPosibles;

    movimientosPosibles = tablero.movimientosPosibles();
    Tablero* auxiliar;

    int mejorH = INT_MAX;

    for (int i = 0; i < movimientosPosibles.size(); i++) {
        cout << "Posible movimiento " << i+1 << ':' << endl;
        auxiliar = new Tablero(tablero);
        //traducción de movimiento en tablero original a tablero nuevo
        movimientosPosibles[i].first = auxiliar->matriz[movimientosPosibles[i].first->partes.front().first][movimientosPosibles[i].first->partes.front().second];
        cout << " - Vehiculo movido: \'" << movimientosPosibles[i].first->letra << '\'' << endl;
        auxiliar->realizarMovimiento(movimientosPosibles[i]);

        int h = auxiliar->calcHeuristica();
        if (h < mejorH) {
            mejorH = h;
        }
        cout << " - h' = " << h << endl;;
        auxiliar->mostrarMatriz();
        delete auxiliar;
        cout << endl;
    }
     cout << " - Mejor heurística es: " << mejorH;

    return 0;
}
