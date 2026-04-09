#include "jugar.h"
#include <unordered_map>
#include "modifiableHeap.h"

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
        this->metodoBusquedaAstar();
        break;
    case 2:
        this->metodoEscaladaSimple();
        break;
    case 3:
        this->metodoEscaladaSimpleEstocastica();
        break;
    case 4:
        this->metodoBusquedaPrimeroMejorAC();
        break;
    case 5:
        this->metodoBusquedaHaz();
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
    for(int i = 0; i < 6; i++){
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

void jugar::mostrarMovimientos(Tablero &t, int &indice, int &heuristica)
{
    indice = 0;
    movimientosPosibles = t.movimientosPosibles();

    heuristica = INT_MAX;

    for (int i = 0; i < movimientosPosibles.size(); i++) {
        cout << "Posible movimiento " << i+1 << ':' << endl;
        Tablero* auxiliar = new Tablero(t);
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

void jugar::mostrarMovimientosAC (int &indice, int &heuristica, Tablero *t, pair<Vehiculo*, Tablero::direccion> *mov)
{
    cout << "Posible movimiento " << indice << ':' << endl;
    cout << " - Vehiculo movido: \'" << mov->first->letra << '\'' << endl;

    int h = calcularHeuristica(t);
    cout << " - h' = " << h << endl;

    t->mostrarMatriz();
    cout << endl;
}



bool jugar::existeEnCerrados(const vector<nodo*>& cerrados, const Tablero& tabActual) {
    for (nodo* n : cerrados) {
        if (n->tab == tabActual) {
            return true;
        }
    }
    return false;
}

void jugar::mostrarAbiertos(priority_queue<nodo*, vector <nodo*>, CompararNodos> abiertos) {
    cout << "\n=== LISTA ABIERTOS ===" << endl;

    while (!abiertos.empty()) {
        nodo *n = abiertos.top();
        cout << n->id << " " << n->idPadre << " " << n->coste << endl;
        abiertos.pop();
        delete n;
    }
}

void jugar::mostrarCerrados(vector <nodo*> cerrados) {
    cout << "\n=== LISTA CERRADOS ===" << endl;

    for (nodo* n : cerrados) {
        cout << n->id << " " << n->idPadre << " " << n->coste << endl;
        delete n;
    }
}

int jugar::calcularHeuristica(Tablero *t)
{
    int h = 0;
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
    int umbral = calcularHeuristica(&tableroInicial);

    while(true){
        int temp = busquedaIDA(tableroInicial, 0, umbral);

        if(temp == -1){
            break;
        }else if(temp == INT_MAX){
            cout << "\nNo se ha encontrado solucion." << endl;
        }else{
            umbral = temp;
        }
    }
}

int jugar::busquedaIDA(Tablero &tableroActual, int g, int umbral)
{
    int f = g + calcularHeuristica(&tableroActual);

    if(f > umbral){
        return f;
    }

    if(tableroActual.comprobarFin()){
        tableroActual.mostrarMatriz();
        cout << "Tablero final encontrado." << endl;
        return -1;
    }else{
        int minimo = INT_MAX;

        vector<pair<Vehiculo*, Tablero::direccion>> mov = tableroActual.movimientosPosibles();

        for(pair<Vehiculo*, Tablero::direccion> m : mov){
            Tablero auxiliar = tableroActual;

            m.first = auxiliar.matriz[m.first->partes.front().first][m.first->partes.front().second];

            auxiliar.realizarMovimiento(m);

            int temp = busquedaIDA(auxiliar, g + 1, umbral);

            if(temp == -1){
                cout << "\nPaso en profundidad " << g + 1 << ", Vehiculo movido: '" << m.first->letra << "'" << endl;
                auxiliar.mostrarMatriz();
                return -1;
            }

            if(temp < minimo){
                minimo = temp;
            }
        }

        return minimo;
    }
}

void jugar::metodoEscaladaSimple()
{
    int i = 0;
    int h_siguiente;

    int h_actual = calcularHeuristica(&tableroInicial);

    while(!tableroInicial.comprobarFin()){
        this->mostrarMovimientos(this->tableroInicial, i, h_siguiente);

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
    vector <nodo*> cerrados;

    nodo *inicial = new nodo {cont++, -1, calcularHeuristica(&tableroInicial), tableroInicial};
    abiertos.push(inicial);

    while (!abiertos.empty()) {
        nodo *actual = abiertos.top();
        abiertos.pop();

        if (existeEnCerrados(cerrados, actual->tab)) {
            delete actual;
            continue;
        }
        cerrados.push_back(actual);
        actual->tab.mostrarMatriz();
        cout << endl;

        if(actual->tab.comprobarFin()) {
            cout << "¡Objetivo alcanzado!" << endl;
            break;
        }

        vector<pair<Vehiculo*, Tablero::direccion>> mov = actual->tab.movimientosPosibles();
        int i = 0;
        for (pair<Vehiculo*, Tablero::direccion> m : mov) {
            Tablero auxiliar = actual->tab;
            m.first = auxiliar.matriz[m.first->partes.front().first][m.first->partes.front().second];
            auxiliar.realizarMovimiento(m);

            if (!existeEnCerrados(cerrados, auxiliar)) {
                i++;
                int hAuxiliar = calcularHeuristica(&auxiliar);
                //this->mostrarMovimientosAC(i, hAuxiliar, &auxiliar, &m);
                cont++;
                nodo *hijo = new nodo(cont, actual->id, hAuxiliar, auxiliar);
                abiertos.push(hijo);
            }
        }
    }

    mostrarAbiertos(abiertos);

    for (nodo* n : cerrados) {
        delete n;
    }
}

struct Anodo{
    Anodo* padre;
    std::vector<Anodo*> hijos;
    Tablero tab;
    float h;
    int g;
    Anodo(){};
    Anodo(Anodo* padre, Tablero const& tab, float h = 0, int g = 0)
        : padre(padre), tab(tab), h(h), g(g){}
    void cascadeG(){
        for(Anodo* n : hijos){
            n->g = g+1;
            n->cascadeG();
        }
    }
};

void jugar::metodoBusquedaAstar()
{
    ModifiableHeap<Anodo*> abiertos;
    unordered_map<std::string,Anodo*> cerrados;
    Anodo* nodoActual = new Anodo(nullptr, tableroInicial, calcularHeuristica(&tableroInicial), 0);
    
    abiertos.push(nodoActual);
    while(!abiertos.empty()){
        nodoActual = abiertos.top();
        abiertos.pop();
        cerrados.insert({nodoActual->tab.key(), nodoActual});
        
        //generar hijos
        vector<pair<Vehiculo*, Tablero::direccion>> posibles = nodoActual->tab.movimientosPosibles();
        
        for(auto mov : posibles){
            Tablero newTab(nodoActual->tab);
            mov.first = newTab.matriz[mov.first->partes.front().first][mov.first->partes.front().second];//traducción de movimiento a nuevo tablero
            newTab.realizarMovimiento(mov);
            
            if(cerrados.count(newTab.key()) > 0){//está el nuevo nodo en cerrados?
                Anodo* old = cerrados.find(newTab.key())->second;
                if(old->g > nodoActual->g+1){//old es peor, se sobreescribe
                    old->g = nodoActual->g+1;
                    old->padre = nodoActual;
                    old->cascadeG();
                }//si el nuevo es peor o igual lo ignoramos
            }else{
                //buscamos repetidos
                bool repe = false;
                int repeIndex = 0;
                for(; repeIndex < abiertos.size() && !repe; repeIndex++)
                    if(newTab == abiertos[repeIndex]->tab)
                        repe = true;
                repeIndex--;//el for añade 1 de más antes de salir
                
                if(repe){//esta repetido
                    if(nodoActual->g+1 < abiertos[repeIndex]->g){//es el nuevo mejor?
                        //si sí, sobreescribe
                        abiertos[repeIndex]->g = nodoActual->g+1;
                        //lo quitamos de la lista de hijos de su padre
                        vector<Anodo*> & hijosPadre = abiertos[repeIndex]->padre->hijos;
                        auto it = hijosPadre.begin();
                        for(; it != hijosPadre.end(); it++)
                            if(*it == abiertos[repeIndex])
                                break;
                        hijosPadre.erase(it);
                        abiertos[repeIndex]->padre = nodoActual;
                        nodoActual->hijos.push_back(abiertos[repeIndex]);
                        abiertos.upheap(repeIndex);//lo hemos mejorado por lo que ascenderá
                    }//si es peor lo descartamos
                }else{
                    if(newTab.comprobarFin()){//solución encontrada!
                        cout << "¡Objetivo alcanzado!" << endl;
                        //mostramos solución
                        cout << "Solución:\n";
                        vector<Anodo*> sol;
                        for(Tablero* ctab = &newTab; !(*ctab == tableroInicial); nodoActual = nodoActual->padre){
                            sol.push_back(nodoActual);
                            ctab = &nodoActual->tab;
                        }
                        //retrazamos desde el principio
                        for(auto it = sol.rbegin(); it != sol.rend(); it++){
                            cout << endl;
                            (*it)->tab.mostrarMatriz();
                        }
                        //limpiamos los abiertos que queden:
                        while(!abiertos.empty()){
                            delete abiertos.top();
                            abiertos.pop();
                        }
                        break;
                    }else{//el nuevo nodo no es el final
                        Anodo* newNodo = new Anodo(nodoActual, newTab, calcularHeuristica(&newTab), nodoActual->g+1);
                        nodoActual->hijos.push_back(newNodo);
                        abiertos.push(newNodo);
                    }
                }
            }
        }
    }
    for(auto pair : cerrados)
        delete pair.second;
}

struct Hnodo{
    Hnodo* padre;
    Tablero tab;
    float h;
    Hnodo(Hnodo* padre, Tablero const& tab, float h)
        : padre(padre), tab(tab), h(h){}
    static bool comp(Hnodo* lhs, Hnodo* rhs){
        return lhs->h < rhs->h;
    }
};
template<> class std::less<Hnodo*>{
public:
    constexpr bool operator()(Hnodo* lhs, Hnodo* rhs) const{
        return lhs->h < rhs->h;
    }
};

bool jugar::metodoBusquedaHaz()
{
    int width = 2;
    string in;
    cout << "Introducir valor de anchura del haz (Pulsar enter para valor predeterminado (2)):" << endl;
    getline(std::cin, in);
    if (in.empty()) {
        width = 2;
    }else{
        width = stoi(in);
    }
    
    bool found = false;
    vector<Hnodo*> cerrados;//(para limpieza tras retrazar la solución)
    priority_queue<Hnodo*, vector<Hnodo*>, std::less<Hnodo*>> abiertos;
    
    abiertos.push(new Hnodo(nullptr, tableroInicial, calcularHeuristica(&tableroInicial)));
    
    while(!abiertos.empty()){
        Hnodo* actual = abiertos.top();
        abiertos.pop();
        cerrados.push_back(actual);
        priority_queue<Hnodo*, vector<Hnodo*>, std::less<Hnodo*>> hijos;
        
        //generamos los nodos hijos y calculamos su heuristica
        vector<pair<Vehiculo*, Tablero::direccion>> posibles = actual->tab.movimientosPosibles();
        for(auto mov : posibles){
            Tablero newTab(actual->tab);
            mov.first = newTab.matriz[mov.first->partes.front().first][mov.first->partes.front().second];//traducción de movimiento a nuevo tablero
            newTab.realizarMovimiento(mov);
            if(newTab.comprobarFin()){
                cout << "¡Objetivo alcanzado!" << endl;
                found = true;
                //mostramos solución
                cout << "Solución:\n";
                vector<Hnodo*> sol;
                for(Tablero* ctab = &newTab; !(*ctab == tableroInicial); actual = actual->padre){
                    sol.push_back(actual);
                    ctab = &actual->tab;
                }
                //retrazamos desde el principio
                for(auto it = sol.rbegin(); it != sol.rend(); it++){
                    cout << endl;
                    (*it)->tab.mostrarMatriz();
                }
                //limpiamos los abiertos que queden e hijos ahora innecesarios:
                while(!abiertos.empty()){
                    delete abiertos.top();
                    abiertos.pop();
                }
                while(!hijos.empty()){
                    delete hijos.top();
                    hijos.pop();
                }
                break;
            }
            hijos.push(new Hnodo(actual, newTab, calcularHeuristica(&newTab)));
        }
        
        //añadimos los <width> mejores nodos a la lista de abiertos
        for(int i = 0; i < width && !hijos.empty(); i++){
            abiertos.push(hijos.top());
            hijos.pop();
        }
        //descartamos el resto de hijos
        while(!hijos.empty()){
            delete hijos.top();
            hijos.pop();
        }
    }
    //limpiamos
    for(int i = 0; i < cerrados.size(); i++)
        delete cerrados[i];
    if(!found){
        cout << "No se ha encontrado la solución...\n";
    }
    return found;
}
