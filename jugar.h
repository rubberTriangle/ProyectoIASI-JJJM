#ifndef JUGAR_H
#define JUGAR_H

#include <iostream>
#include <climits>
#include "Tablero.h"

class jugar {
private:
    int hSelecIndex;
    string heuristicas[2] = {"Distancia Manhattan simple", "Distancia Manhattan por vehículos"};
    string heuristicaSeleccionada;
    int sSelecIndex;
    string resolucion[2] = {"Distancia Manhattan simple", "Distancia Manhattan por vehículos"};
    string resolucionSeleccionada;

public:
    jugar();
    void elegirHeuristica();
    void elegirMetodo();
};

#endif // JUGAR_H
