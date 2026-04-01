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
    string resolucion[5] = {"IDA", "A*", "Escalada simple", "Escalada simple estocástica", "Búsqueda primero mejor con lista de abiertos y cerrados"};
    string resolucionSeleccionada;

    string alfa;

    vector<pair<Vehiculo*, Tablero::direccion>> movimientosPosibles;

public:
    jugar();
    void elegirHeuristica();
    void elegirMetodo();
    void mostrarMovimientos(int &i, int &h);
    bool mostrarUnMovimientoEstocastica(int &i, int &h);
    int calcularHeuristica(Tablero *t);
    void metodoIDA();
    void metodoEscaladaSimple();
    void metodoEscaladaSimpleEstocastica();
    void metodoBusquedaPrimeroMejorAC();
};

#endif // JUGAR_H
