#ifndef JUGAR_H
#define JUGAR_H

#include <iostream>
#include <climits>
#include "Tablero.h"

class jugar {
private:
    Tablero tableroInicial;
    int hSelecIndex;
    string heuristicas[2] = {"Distancia Manhattan simple", "Distancia Manhattan por vehículos"};
    string heuristicaSeleccionada;
    int sSelecIndex;
    string resolucion[2] = {"IDA", "A*"};
    string resolucionSeleccionada;

public:
    jugar();
    void elegirHeuristica();
    void elegirMetodo();
    void mostrarMovimientos();
    void metodoIDA();
};

#endif // JUGAR_H
