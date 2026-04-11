#ifndef JUGAR_H
#define JUGAR_H

#include <chrono>
#include <iostream>
#include <climits>
#include <queue>
#include "Tablero.h"

const int MAX_CONT = 100000;//máximo para failsafes en caso de que algoritmos no salgan si no encuentran solución

class jugar {
private:
    Tablero tableroInicial;
    int hSelecIndex;
    string heuristicas[2] = {"Distancia Manhattan simple", "Distancia Manhattan por vehículos"};
    string heuristicaSeleccionada;
    int sSelecIndex;
    string resolucion[6] = {"IDA", "A*", "Escalada simple", "Escalada simple estocástica", "Búsqueda primero mejor con lista de abiertos y cerrados", "Búsqueda de Haz (Beam Search)"};
    string resolucionSeleccionada;

    string alfa;
    int cont;

    vector<pair<Vehiculo*, Tablero::direccion>> movimientosPosibles;
    struct nodo {
        int id;
        int idPadre;
        int coste;
        Tablero tab;
        char vehiculo;

        nodo(int _id, int _idPadre, int _coste, Tablero _tab, char _vehiculo)
            : id(_id), idPadre(_idPadre), coste(_coste), tab(_tab), vehiculo(_vehiculo) {}

        nodo() : id(0), idPadre(-1), coste(0), vehiculo(' ') {}
    };
    struct CompararNodos {
        bool operator()(const nodo* a, const nodo* b) {
            return a->coste > b->coste;
        }
    };
    
    int busquedaIDA(Tablero &tableroActual, int g, int umbral, vector<char> &path);
    inline void printElapsedTime(chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, ratio<1,1000000000>>> begin);

public:
    jugar();
    void elegirHeuristica();
    void elegirMetodo();
    void mostrarMovimientos(Tablero &t, int &h);
    bool mostrarUnMovimientoEstocastica(int &i, int &h);
    bool existeEnCerrados(const vector<nodo*>& cerrados, const Tablero& tabActual);
    bool existeEnAbiertos(priority_queue<nodo*, vector <nodo*>, CompararNodos> abiertos, const Tablero& tabActual);
    void buscarNodosSolucion (vector <nodo*> cerrados);
    void mostrarAbiertos(priority_queue<nodo*, vector <nodo*>, CompararNodos> abiertos);
    void mostrarCerrados(vector<nodo *> cerrados);
    int calcularHeuristica(Tablero *t);
    void metodoIDA();
    void metodoEscaladaSimple();
    void metodoEscaladaSimpleEstocastica();
    void metodoBusquedaPrimeroMejorAC();
    void metodoBusquedaAstar();
    bool metodoBusquedaHaz();
};

#endif // JUGAR_H
