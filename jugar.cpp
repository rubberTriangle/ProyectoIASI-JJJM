#include "jugar.h"

jugar::jugar() {
    this->elegirHeuristica();
    this->elegirMetodo();
    switch(stoi(this->resolucionSeleccionada)){
    case 0:
        this->metodoIDA();
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        cout << "El método seleccionado no está disponible" << endl;
    }

    this->mostrarMovimientos();
}

void jugar::elegirHeuristica () {
    cout << "Elige la heurística a utilizar: (Presiona Enter para valor predeterminado)" << endl;
    for(int i = 0; i < 2; i++){
        cout << "[" << i << "] " << heuristicas[i] << endl;
    }

    getline(std::cin, heuristicaSeleccionada);

    if (heuristicaSeleccionada.empty()) {
        heuristicaSeleccionada = "0";
    }
    hSelecIndex = std::stoi(heuristicaSeleccionada);
}

void jugar::elegirMetodo () {

    cout << "Elige el método de resolución a utilizar: (Presiona Enter para valor predeterminado)" << endl;
    for(int i = 0; i < 2; i++){
        cout << "[" << i << "] " << resolucion[i] << endl;
    }

    getline(std::cin, resolucionSeleccionada);

    if (resolucionSeleccionada.empty()) {
        resolucionSeleccionada = "0";
    }
    sSelecIndex = std::stoi(resolucionSeleccionada);

    cout << "Matriz cargada:\n";
    tableroInicial.mostrarMatriz();
    cout << endl;
}

vector<pair<Vehiculo*, Tablero::direccion>> jugar::mostrarMovimientos()
{
    vector<pair<Vehiculo*, Tablero::direccion>> movimientosPosibles;

    movimientosPosibles = tableroInicial.movimientosPosibles();
    Tablero* auxiliar;

    int mejorH = INT_MAX;

    for (int i = 0; i < movimientosPosibles.size(); i++) {
        cout << "Posible movimiento " << i+1 << ':' << endl;
        auxiliar = new Tablero(tableroInicial);
        //traducción de movimiento en tablero original a tablero nuevo
        movimientosPosibles[i].first = auxiliar->matriz[movimientosPosibles[i].first->partes.front().first][movimientosPosibles[i].first->partes.front().second];
        cout << " - Vehiculo movido: \'" << movimientosPosibles[i].first->letra << '\'' << endl;
        auxiliar->realizarMovimiento(movimientosPosibles[i]);

        int h;
        switch(hSelecIndex){
        case 0:
            h = auxiliar->calcHeuristicaSimple();
            break;
        case 1:
            h = auxiliar->calcHeuristicaVehiculos();
            break;
        }

        if (h < mejorH) {
            mejorH = h;
        }
        cout << " - h' = " << h << endl;;
        auxiliar->mostrarMatriz();
        delete auxiliar;
        cout << endl;
    }
    cout << " - Mejor heurística es: " << mejorH;
}

void jugar::metodoIDA()
{
    vector<pair<Vehiculo*, Tablero::direccion>>movimientosPosibles = this->mostrarMovimientos();
}
