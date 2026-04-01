#include "jugar.h"

jugar::jugar() {
    alfa = 3;
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
    case 3:
        this->metodoEscaladaSimpleEstocastica();
        break;
    case 4:
        this->metodoBusquedaPrimeroMejorAC();
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
    for(int i = 0; i < 5; i++){
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
        cout << " - Mejor heurística es: " << heuristica << endl;
    }
}

bool jugar::mostrarUnMovimientoEstocastica(int &indice, int &heuristica)
{
    bool encontrado = false;
    int i = 0;

    if(!this->tableroInicial.comprobarFin()){
        movimientosPosibles = tableroInicial.movimientosPosibles();

        while (i < movimientosPosibles.size() && !encontrado) {

            Tablero *auxiliar = new Tablero(tableroInicial);
            //traducción de movimiento en tablero original a tablero nuevo
            movimientosPosibles[i].first = auxiliar->matriz[movimientosPosibles[i].first->partes.front().first][movimientosPosibles[i].first->partes.front().second];
            auxiliar->realizarMovimiento(movimientosPosibles[i]);

            int h = calcularHeuristica(auxiliar);

            if (h < heuristica && (h + stoi(alfa)) <= heuristica) {
                encontrado = true;
                heuristica = h;
                indice = i;
                cout << " - h' = " << h << endl;;
                auxiliar->mostrarMatriz();
                cout << endl;
            }
            delete auxiliar;
            i++;
        }
        if (!encontrado) {
            cout << "No tiene solución utilizando el método de escalada simple estocástica" << endl;
        }
    }
    return encontrado;
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

void jugar::metodoEscaladaSimpleEstocastica()
{
    cout << "Introducir valor de alfa (Pulsar enter para valor predeterminado" << endl;
    getline(std::cin, alfa);
    if (alfa.empty()) {
        alfa = "3";
    }
    cout << "Alfa = " << alfa << endl;

    int h = INT_MAX;

    while (true) {

        int i = 0;
        if (this->mostrarUnMovimientoEstocastica(i, h)) {
            vector<pair<Vehiculo*, Tablero::direccion>> mov = this->tableroInicial.movimientosPosibles();
            this->tableroInicial.realizarMovimiento(mov[i]);

            if(tableroInicial.comprobarFin()) {
                cout << "¡Objetivo alcanzado!" << endl;
            break;
            }
        }
        else {
        // Si no hay más movimientos posibles, salimos del bucle
            break;
        }
    }
}

void jugar::metodoBusquedaPrimeroMejorAC()
{
    //priority_queue abiertos = new priority_queue ();
    //queue cerrados  = new queue ();
    struct nodo {
        int id;
        int idPadre;
        int coste;
    };

}
