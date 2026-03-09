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
        this->metodoEscaladaSimple();
        break;
    default:
        cout << "El método seleccionado no está disponible" << endl;
    }
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
    for(int i = 0; i < 3; i++){
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

void jugar::mostrarMovimientos(int &indice, int &heuristica)
{
    indice = 0;

    if(!this->tableroInicial.comprobarFin()){
        movimientosPosibles = tableroInicial.movimientosPosibles();
        Tablero* auxiliar;

        heuristica = INT_MAX;

        for (int i = 0; i < movimientosPosibles.size(); i++) {
            cout << "Posible movimiento " << i+1 << ':' << endl;
            auxiliar = new Tablero(tableroInicial);
            //traducción de movimiento en tablero original a tablero nuevo
            movimientosPosibles[i].first = auxiliar->matriz[movimientosPosibles[i].first->partes.front().first][movimientosPosibles[i].first->partes.front().second];
            cout << " - Vehiculo movido: \'" << movimientosPosibles[i].first->letra << '\'' << endl;
            auxiliar->realizarMovimiento(movimientosPosibles[i]);

            int h = calcularHeuristica(auxiliar);

            if (h < heuristica) {
                heuristica = h;
                indice = i;
            }
            cout << " - h' = " << h << endl;;
            auxiliar->mostrarMatriz();
            delete auxiliar;
            cout << endl;
        }
        cout << " - Mejor heurística es: " << heuristica;
    }
}

int jugar::calcularHeuristica(Tablero *t)
{
    int h;
    switch(hSelecIndex){
    case 0:
        h = t->calcHeuristicaSimple();
        break;
    case 1:
        h = t->calcHeuristicaVehiculos();
        break;
    }

    return h;
}

void jugar::metodoIDA()
{
    int i, h;
    this->mostrarMovimientos(i, h);
}

void jugar::metodoEscaladaSimple()
{
    int i, h;
    while(true){
        this->mostrarMovimientos(i, h);

        Tablero* aux = new Tablero(tableroInicial);
        aux->realizarMovimiento(this->movimientosPosibles[i]);
        this->tableroInicial = *aux;
    }
}
