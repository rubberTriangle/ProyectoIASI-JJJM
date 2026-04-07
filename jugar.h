#ifndef JUGAR_H
#define JUGAR_H

#include <iostream>
#include <climits>
#include <queue>
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
    int cont;

    vector<pair<Vehiculo*, Tablero::direccion>> movimientosPosibles;
    struct nodo {
        int id;
        int idPadre;
        int coste;
        Tablero tab;
    };
    struct CompararNodos {
        bool operator()(const nodo* a, const nodo* b) {
            return a->coste > b->coste;
        }
    };

public:
    jugar();
    void elegirHeuristica();
    void elegirMetodo();
    void mostrarMovimientos(int &i, int &h);
    bool mostrarUnMovimientoEstocastica(int &i, int &h);
    void mostrarMovimientosAC(int &i, int &h, priority_queue <nodo*, vector <nodo*>, CompararNodos> &abiertos, queue <nodo*> &cerrados);
    bool existeEnCerrados(queue<nodo*> cerrados, Tablero tabActual);
    void mostrarAbiertos(priority_queue<nodo*, vector <nodo*>, CompararNodos> abiertos);
    void mostrarCerrados(queue <nodo*> cerrados);
    int calcularHeuristica(Tablero *t);
    void metodoIDA();
    void metodoEscaladaSimple();
    void metodoEscaladaSimpleEstocastica();
    void metodoBusquedaPrimeroMejorAC();
};

#endif // JUGAR_H
