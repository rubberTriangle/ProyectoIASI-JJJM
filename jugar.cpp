#include "jugar.h"

jugar::jugar() {
    alfa = 3;
    cont = 0;
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
    if(!this->tableroInicial.comprobarFin()){
        indice = 0;
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

void jugar::mostrarMovimientosAC (int &indice, int &heuristica, priority_queue <nodo*, vector <nodo*>, CompararNodos> &abiertos, queue <nodo*> &cerrados)
{
    if(!this->tableroInicial.comprobarFin()){
        movimientosPosibles = tableroInicial.movimientosPosibles();

        for (int i = 0; i < movimientosPosibles.size(); i++) {
            Tablero *auxiliar;
            cout << "Posible movimiento " << i+1 << ':' << endl;
            auxiliar = new Tablero(tableroInicial);
            //traducción de movimiento en tablero original a tablero nuevo
            movimientosPosibles[i].first = auxiliar->matriz[movimientosPosibles[i].first->partes.front().first][movimientosPosibles[i].first->partes.front().second];
            auxiliar->realizarMovimiento(movimientosPosibles[i]);

            int h = calcularHeuristica(auxiliar);

            nodo* hijo = new nodo();
            hijo->id = cont++;
            hijo->idPadre = indice;
            hijo->coste = h;
            hijo->matrizTablero = auxiliar->matriz;

            if (!existeEnCerrados(cerrados, hijo->matrizTablero)) {
                abiertos.push(hijo);
            }
            else {
                delete hijo;
            }
        }

    }
}



bool jugar::existeEnCerrados(queue<nodo*> cerrados, const vector<vector<Vehiculo*>>& matrizActual) {
    while (!cerrados.empty()) {
        if (cerrados.front()->matrizTablero == matrizActual) {
            return true;
        }
        cerrados.pop();
    }
    return false;
}

void jugar::mostrarAbiertos(priority_queue<nodo*, vector <nodo*>, CompararNodos> abiertos) {
    cout << "\n=== LISTA ABIERTOS ===" << endl;

    while (!abiertos.empty()) {
        nodo *n = abiertos.top();
        cout << n->id << " " << n->idPadre << " " << n->coste << endl;
        abiertos.pop();
    }
}

void jugar::mostrarCerrados(queue <nodo*> cerrados) {
    cout << "\n=== LISTA CERRADOS ===" << endl;

    while (!cerrados.empty()) {
        nodo *n = cerrados.front();
        cout << n->id << " " << n->idPadre << endl;
        cerrados.pop();
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
    int i = 0;
    int h_siguiente;

    int h_actual = calcularHeuristica(&tableroInicial);

    while(!tableroInicial.comprobarFin()){
        this->mostrarMovimientos(i, h_siguiente);

        if (h_siguiente >= h_actual) {
            cout << "\n¡Atascado en un óptimo local! No hay movimientos que mejoren la heurística." << endl;
            break;
        }

        h_actual = h_siguiente;

        this->tableroInicial.realizarMovimiento(this->tableroInicial.movimientosPosibles()[i]);
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
            break;
        }
    }
}

void jugar::metodoBusquedaPrimeroMejorAC()
{
    priority_queue <nodo*, vector <nodo*>, CompararNodos> abiertos;
    queue <nodo*> cerrados;
    int h = 0;

    nodo *inicial = new nodo {cont++, -1, calcularHeuristica(&tableroInicial), tableroInicial.matriz};
    abiertos.push(inicial);

    while (!abiertos.empty()) {
        nodo *actual = abiertos.top();
        abiertos.pop();

        if (existeEnCerrados(cerrados, actual->matrizTablero)) {
            continue;
        }

        this->tableroInicial.matriz = actual->matrizTablero;

        if(this->tableroInicial.comprobarFin()) {
            cerrados.push(actual);
            cout << "¡Objetivo alcanzado!" << endl;
            break;
        }

        cerrados.push(actual);

        vector<pair<Vehiculo*, Tablero::direccion>> mov = this->tableroInicial.movimientosPosibles();
        for (pair<Vehiculo*, Tablero::direccion> m : mov) {
            Tablero auxiliar = tableroInicial;
            auxiliar.realizarMovimiento(m);

            if (!existeEnCerrados(cerrados, auxiliar.matriz)) {
                nodo *hijo = new nodo();
                hijo->id = cont++;
                hijo->idPadre = actual->id;
                hijo->matrizTablero = auxiliar.matriz;
                hijo->coste = calcularHeuristica(&auxiliar);
                abiertos.push(hijo);
            }
        }
    }
    while (!abiertos.empty()) {
        nodo *n = abiertos.top();
        delete n;
        abiertos.pop();
    }
    while (!cerrados.empty()) {
        nodo *n = cerrados.front();
        delete n;
        cerrados.pop();
    }
}
