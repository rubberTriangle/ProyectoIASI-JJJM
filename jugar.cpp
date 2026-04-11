#include "jugar.h"
#include <unordered_map>
#include "modifiableHeap.h"
#include <chrono>

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

    cout << "Nodos generados: " << cont << endl;
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
    cout << "\nEl método seleccionado es: " << resolucion[sSelecIndex] << endl;

    cout << "Matriz cargada:\n";
    tableroInicial.mostrarMatriz();
    cout << endl;
}

void jugar::mostrarMovimientos(Tablero &t, int &heuristica)
{
    int indice = 0;
    movimientosPosibles = t.movimientosPosibles();

    heuristica = INT_MAX;

    for (int i = 0; i < movimientosPosibles.size(); i++) {
        cout << "Posible movimiento " << i+1 << ':' << endl;
        Tablero* auxiliar = new Tablero(t);
        //traducción de movimiento en tablero original a tablero nuevo
        movimientosPosibles[i].first = auxiliar->matriz[movimientosPosibles[i].first->partes.front().first][movimientosPosibles[i].first->partes.front().second];
        cout << " - Vehiculo movido: \'" << movimientosPosibles[i].first->letra << '\'' << endl;
        auxiliar->realizarMovimiento(movimientosPosibles[i]);
        cont++;

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

bool jugar::existeEnCerrados(const vector<nodo*>& cerrados, const Tablero& tabActual) {
    for (nodo* n : cerrados) {
        if (n->tab == tabActual) {
            return true;
        }
    }
    return false;
}

bool jugar::existeEnAbiertos(priority_queue<nodo*, vector<nodo*>, CompararNodos> abiertos, const Tablero& tabActual) {
    while (!abiertos.empty()) {
        if (abiertos.top()->tab == tabActual)
            return true;
        abiertos.pop();
    }
    return false;
}

void jugar::buscarNodosSolucion (vector <nodo*> cerrados) {
    nodo *actual = cerrados.back();
    vector<nodo*> camino;

    while (actual != nullptr) {
        camino.push_back(actual);
        nodo* padre = nullptr;
        for (nodo* n : cerrados) {
            if (n->id == actual->idPadre) {
                padre = n;
                break;
            }
        }
        actual = padre;
    }
    reverse(camino.begin(), camino.end());
    cout << "\n=== SOLUCIÓN ===" << endl;
    camino[0]->tab.mostrarMatriz();
    cout << endl;
    for (int i = 1; i < camino.size(); i++) {
        cout << "Vehículo movido: " << camino[i]->vehiculo << endl;
        camino[i]->tab.mostrarMatriz();
        cout << endl;
    }
    cout << "Solución: ";
    for (int i = 1; i < camino.size(); i++) {
        cout << camino[i]->vehiculo;
        if (i < camino.size() - 1) {
            cout << ", ";
        }
    }
}

void jugar::mostrarAbiertos(priority_queue<nodo*, vector <nodo*>, CompararNodos> abiertos) {
    cout << "\n=== LISTA ABIERTOS ===" << endl;

    while (!abiertos.empty()) {
        nodo *n = abiertos.top();
        cout << n->id << " (" << n->idPadre << ", " << n-> coste << ")";
        if (!abiertos.empty()) {
            cout << ", ";
        }
        abiertos.pop();
    }
    cout << endl;
}

void jugar::mostrarCerrados(vector <nodo*> cerrados) {
    cout << "\n=== LISTA CERRADOS ===" << endl;

    for (int i = 0; i < cerrados.size(); i++) {
        cout << cerrados[i]->id << " (" << cerrados[i]->idPadre << ")";
        if (i < cerrados.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl << endl;
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
    auto begin = std::chrono::high_resolution_clock::now();
    int umbral = calcularHeuristica(&tableroInicial);

    while(true){
        int temp = busquedaIDA(tableroInicial, 0, umbral);

        if(temp == -1){
            break;
        }else if(temp == -2){
            cout << "\nLímite de nodos generados excedido, este tablero probablemente no tenga solución.\n";
            break;
        }else if(temp == INT_MAX){
            cout << "\nNo se ha encontrado solucion." << endl;
        }else{
            umbral = temp;
        }
    }
    printElapsedTime(begin);
}

int jugar::busquedaIDA(Tablero &tableroActual, int g, int umbral)
{
    int f = g + calcularHeuristica(&tableroActual);

    if(f > umbral){
        return f;
    }
    if(cont > MAX_CONT)
        return -2;

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
            cont++;

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

void jugar::printElapsedTime(chrono::time_point<chrono::system_clock, std::chrono::duration<long, ratio<1, 1000000000> > > begin)
{
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("\n%f seconds.\n", elapsed.count() * 1e-9);
}

void jugar::metodoEscaladaSimple()
{
    vector <char> solucion;
    auto begin = std::chrono::high_resolution_clock::now();
    int h_actual = calcularHeuristica(&tableroInicial);
    Tablero tableroAuxiliar = tableroInicial;

    while(!tableroAuxiliar.comprobarFin()){
        auto m = tableroAuxiliar.movimientosPosibles();

        if (m.empty()) {
            cout << "No hay movimientos posibles desde este estado." << endl;
            break;
        }

        int min_h = 0;
        int mejor_h_vecino = INT_MAX;

        for(size_t i = 0; i < m.size(); i++){
            Tablero aux = tableroAuxiliar;
            aux.realizarMovimiento({aux.matriz[m[i].first->partes.front().first][m[i].first->partes.front().second], m[i].second});

            int h_eval = calcularHeuristica(&aux);

            if(h_eval < mejor_h_vecino){
                mejor_h_vecino = h_eval;
                min_h = i;
            }
        }

        if (mejor_h_vecino >= h_actual) {
            cout << "\n¡Atascado en un óptimo local! No hay movimientos que mejoren la heurística." << endl;
            break;
        }

        h_actual = mejor_h_vecino;
        tableroAuxiliar.realizarMovimiento(m[min_h]);

        cout << "Movimiento realizado:" << endl;
        cout << "Vehículo movido: " << m[min_h].first->letra << ", heurística: " << h_actual << endl;
        solucion.push_back((m[min_h].first->letra));
        tableroAuxiliar.mostrarMatriz();

        cont++;
    }

    if(tableroAuxiliar.comprobarFin()){
        cout << "Objetivo alcanzado" << endl;
        cout << "\nSolución: ";
        for (int i = 0; i < solucion.size(); i++) {
            cout << solucion[i];
            if (i < solucion.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }else{
        cout << "No se ha alcanzado el objetivo" << endl;
    }

    printElapsedTime(begin);
}

void jugar::metodoEscaladaSimpleEstocastica()
{
    vector <char> solucion;
    cout << "Introducir valor de alfa (Pulsar enter para valor predeterminado):" << endl;
    getline(std::cin, alfa);
    if (alfa.empty()) {
        alfa = "3";
    }
    cout << "Alfa = " << alfa << endl;
    
    auto begin = std::chrono::high_resolution_clock::now();
    int h = INT_MAX;

    while (true) {

        int i = 0;
        if (this->mostrarUnMovimientoEstocastica(i, h)) {
            vector<pair<Vehiculo*, Tablero::direccion>> mov = this->tableroInicial.movimientosPosibles();
            this->tableroInicial.realizarMovimiento(mov[i]);
            cout << "Vehículo movido: " << mov[i].first->letra << endl;
            solucion.push_back(mov[i].first->letra);
            cont++;

            if(tableroInicial.comprobarFin()) {
                cout << "¡Objetivo alcanzado!" << endl;
                cout << "\nSolución: ";
                for (int i = 0; i < solucion.size(); i++) {
                    cout << solucion[i];
                    if (i < solucion.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            break;
            }
        }
        else {
            break;
        }
    }
    printElapsedTime(begin);
}

void jugar::metodoBusquedaPrimeroMejorAC()
{
    auto begin = std::chrono::high_resolution_clock::now();
    priority_queue <nodo*, vector <nodo*>, CompararNodos> abiertos;
    vector <nodo*> cerrados;

    nodo *inicial = new nodo {cont++, -1, calcularHeuristica(&tableroInicial), tableroInicial, ' '};
    abiertos.push(inicial);

    while (!abiertos.empty()) {
        nodo *actual = abiertos.top();
        abiertos.pop();

        if (existeEnCerrados(cerrados, actual->tab)) {
            continue;
        }
        if (existeEnAbiertos(abiertos, actual->tab)) {
            continue;
        }

        cerrados.push_back(actual);
        if (actual->vehiculo != ' ') {
            cout << "Vehículo movido: " << actual->vehiculo << endl;
        }
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
                nodo *hijo = new nodo(cont++, actual->id, hAuxiliar, auxiliar, m.first->letra);
                abiertos.push(hijo);
            }
        }
        mostrarAbiertos(abiertos);
        mostrarCerrados(cerrados);
    }
    nodo *n = cerrados.back();
    if (n->tab.comprobarFin()) {
        buscarNodosSolucion(cerrados);
    }
    else {
        cout << "No se ha encontrado la solución." << endl;
    }

    printElapsedTime(begin);
}

struct Anodo{
    char vehiculoMovido;
    Anodo* padre;
    std::vector<Anodo*> hijos;
    Tablero tab;
    float h;
    int g;
    Anodo(){};
    Anodo(Anodo* padre, Tablero const& tab, char vehiculoMovido, float h = 0, int g = 0)
        : padre(padre), tab(tab), vehiculoMovido(vehiculoMovido), h(h), g(g){}
    void cascadeG(){
        for(Anodo* n : hijos){
            n->g = g+1;
            n->cascadeG();
        }
    }
    constexpr float f() const{
        return h+g;
    }
};
template<> class std::greater<Anodo*>{
public:
    constexpr bool operator()(Anodo* lhs, Anodo* rhs) const{
        return lhs->f() > rhs->f();
    }
};

void jugar::metodoBusquedaAstar()
{
    auto begin = std::chrono::high_resolution_clock::now();
    ModifiableHeap<Anodo*, std::greater<Anodo*>> abiertos;
    unordered_map<std::string,Anodo*> cerrados;
    if(tableroInicial.comprobarFin()){//comprobamos edge case de que nos den un tablero solucionado
        cout << "El tablero ya está solucionado.\n";
    }else{
        Anodo* nodoActual = new Anodo(nullptr, tableroInicial, ' ', calcularHeuristica(&tableroInicial), 0);
        bool solucionado = false;
        
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
                cont++;
                
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
                            solucionado = true;
                            //mostramos solución
                            cout << "Solución:\n";
                            vector<Anodo*> sol;
                            vector<char> path;
                            for(Tablero* ctab = &newTab; nodoActual->padre != nullptr; nodoActual = nodoActual->padre){
                                sol.push_back(nodoActual);
                                ctab = &nodoActual->tab;
                            }
                            //retrazamos desde el principio
                            for(auto it = sol.rbegin(); it != sol.rend(); it++){
                                cout << "Vehiculo movido: " << (*it)->vehiculoMovido << endl;
                                (*it)->tab.mostrarMatriz();
                                cout << endl;
                                path.push_back((*it)->vehiculoMovido);
                            }
                            //mostramos final
                            cout << "Vehiculo movido: " << mov.first->letra << endl;
                            newTab.mostrarMatriz();
                            cout << endl;
                            
                            cout << "Solución: ";
                            for (int i = 0; i < path.size(); i++)
                                cout << path[i] << ", ";
                            cout << mov.first->letra << endl;
                            
                            //limpiamos los abiertos que queden:
                            while(!abiertos.empty()){
                                delete abiertos.top();
                                abiertos.pop();
                            }
                            break;
                        }else{//el nuevo nodo no es el final
                            Anodo* newNodo = new Anodo(nodoActual, newTab, mov.first->letra, calcularHeuristica(&newTab), nodoActual->g+1);
                            nodoActual->hijos.push_back(newNodo);
                            abiertos.push(newNodo);
                        }
                    }
                }
            }
        }
        for(auto &pair : cerrados)
            delete pair.second;
        if(!solucionado)
            cout << "No se ha encontrado solución.\n";
    }
    printElapsedTime(begin);
}

struct Hnodo{
    char vehiculoMovido;
    Hnodo* padre;
    Tablero tab;
    float h;
    Hnodo(Hnodo* padre, Tablero const& tab, char vehiculoMovido, float h)
        : padre(padre), tab(tab), vehiculoMovido(vehiculoMovido), h(h){}
};
template<> class std::greater<Hnodo*>{
public:
    constexpr bool operator()(Hnodo* lhs, Hnodo* rhs) const{
        return lhs->h > rhs->h;
    }
};

bool jugar::metodoBusquedaHaz()
{
    int width;
    string in;
    cout << "Introducir valor de anchura del haz (Pulsar enter para valor predeterminado (5)):" << endl;
    getline(std::cin, in);
    if (in.empty()) {
        width = 5;
    }else{
        width = stoi(in);
    }
    auto begin = std::chrono::high_resolution_clock::now();
    
    bool found = false;
    unordered_map<string, Hnodo*> cerrados;//(para limpieza, retrazar la solución y evitar ciclos)
    priority_queue<Hnodo*, vector<Hnodo*>, std::greater<Hnodo*>> abiertos;
    
    if(tableroInicial.comprobarFin()){//comprobamos edge case de que nos den un tablero solucionado
        cout << "El tablero ya está solucionado.\n";
        found = true;
    }else{
        abiertos.push(new Hnodo(nullptr, tableroInicial, ' ', calcularHeuristica(&tableroInicial)));
        priority_queue<Hnodo*, vector<Hnodo*>, std::greater<Hnodo*>> hijos;//siguiente capa generada
        
        while(!abiertos.empty()){
            while(!abiertos.empty()){
                Hnodo* actual = abiertos.top();
                abiertos.pop();
                cerrados.insert({actual->tab.key(), actual});
                
                //generamos los nodos hijos y calculamos su heuristica
                vector<pair<Vehiculo*, Tablero::direccion>> posibles = actual->tab.movimientosPosibles();
                for(auto mov : posibles){
                    Tablero newTab(actual->tab);
                    mov.first = newTab.matriz[mov.first->partes.front().first][mov.first->partes.front().second];//traducción de movimiento a nuevo tablero
                    newTab.realizarMovimiento(mov);
                    cont++;
                    if(newTab.comprobarFin()){
                        cout << "¡Objetivo alcanzado!" << endl;
                        found = true;
                        //mostramos solución
                        cout << "Solución:\n";
                        vector<Hnodo*> sol;
                        for(Tablero* ctab = &newTab; actual->padre != nullptr; actual = actual->padre){
                            sol.push_back(actual);
                            ctab = &actual->tab;
                        }
                        //retrazamos desde el principio
                        for(auto it = sol.rbegin(); it != sol.rend(); it++){
                            cout << "Vehiculo movido: " << (*it)->vehiculoMovido << endl;
                            (*it)->tab.mostrarMatriz();
                            cout << endl;
                        }
                        //mostramos final
                        cout << "Vehiculo movido: " << mov.first->letra << endl;
                        newTab.mostrarMatriz();
                        cout << endl;
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
                    if(cerrados.count(newTab.key()) == 0){
                        hijos.push(new Hnodo(actual, newTab, mov.first->letra, calcularHeuristica(&newTab)));
                        cont++;
                    }
                }
                
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
        for(auto &pair : cerrados)
            delete pair.second;
        if(!found)
            cout << "No se ha encontrado la solución.\n";
    }
    printElapsedTime(begin);
    return found;
}
