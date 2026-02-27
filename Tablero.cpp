#include "Tablero.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>

Tablero::Tablero() {
    cout << "Introduce el tablero que quieres utilizar (Presiona Enter para valores predeterminados): " << endl;
    getline(std::cin, nombreTablero);

    if (nombreTablero.empty()) {
        nombreTablero = "HORAPUNTA1";
        ancho = 6;
        alto = 6;
    }else{
        cout << "Introduce la anchura y la altura del tablero del fichero seleccionado: " << endl;
        cin >> ancho >> alto;
    }

    importarMatriz();
}

Tablero::Tablero(const Tablero &other)
    : ancho(other.ancho), alto(other.alto), nombreTablero(other.nombreTablero), posX(other.posX), posZ(other.posZ){
    
    matriz.assign(other.matriz.size(), vector<Vehiculo*>(other.matriz[0].size()));
    for(auto it = other.vehiculos.begin(); it != other.vehiculos.end(); it++){
        Vehiculo* cv = new Vehiculo(**it);
        vehiculos.push_back(cv);
        for(pair<int,int> p : cv->partes)
            matriz[p.first][p.second] = cv;
    }
}

void Tablero::procesarMatriz(vector<vector<char>> const& chmat, vector<vector<Vehiculo*>> & m){
    std::unordered_map<char, Vehiculo*> vmap;
    m.assign(chmat.size(), std::vector<Vehiculo*>(chmat[0].size()));
    for(int i = 0; i < chmat.size(); i++)
        for(int j = 0; j < chmat[0].size(); j++){
            char current = chmat[i][j];
            Vehiculo* cv;
            if(current == 'X'){
                posX = {i, j};
            }else{
                if(current == 'Z')
                    posZ = {i, j};
                if(vmap.count(current) == 0){//no está el vehiculo
                    cv = new Vehiculo(current);
                    vehiculos.push_back(cv);
                    vmap.insert({current, cv});
                }else
                    cv = (*vmap.find(current)).second;
                cv->partes.push_back({i,j});
                m[i][j] = cv;
            }
        }
    
    for(auto it = vehiculos.begin(); it != vehiculos.end(); it++)
        (*it)->calcularDesdePartes();
}

void Tablero::importarMatriz(){
    
    std::vector<std::vector<char>> chmat(ancho, vector<char>(alto));

    ifstream archivo("ficheros/" + nombreTablero + ".txt");
    string linea;

    if(archivo.is_open()){
        int j = 0;
        while(getline(archivo, linea)){
            int k = 0;
            for(int i = 0; i < linea.size(); i++){
                if(linea[i] != ','){
                    chmat[j][k] = linea[i];
                    k++;
                }
            }
            j++;
        }

        archivo.close();
        
        procesarMatriz(chmat, matriz);
    }else{
        cerr << "No se ha abierto el fichero correctamente";
    }
}

void Tablero::mostrarMatriz(){
    for(int j = 0; j < alto; j++){
        for(int i = 0; i < ancho; i++){
            unsigned char C = (j==posX.first && i==posX.second ? 'X' : matriz[j][i]->letra);
            if(C == 'X')
                cout << "\e[38;2;0;0;0;48;2;255;255;255m";
            else if(C == 'Z')
                cout << "\e[38;2;0;0;0;48;2;0;255;255m";
            else{
                int colorBase = (C-65)*14;
                cout << "\e[38;2;" << colorBase%256 << ';' << (colorBase+160)%256 << ';' << (colorBase+80)%256 << 'm';
                //rgb                
            }
            cout << C << "\e[0m" << " ";
        }
        cout << endl;
    }
}

int Tablero::calcHeuristicaSimple() const{
    int hX, hZ;

    hX = abs(posZ.first - posX.first) + abs(posZ.second - posX.second);

    hZ = min({abs(posZ.first - ancho), posZ.first, posZ.second, abs(posZ.second - alto)});

    return hX + hZ;
}

int Tablero::calcHeuristicaVehiculos() const{
    int hZ, hX;
    //distancia entre dos puntos en un mismo eje horizontal o vertical medida en vehiculos entre medias repetidos
    auto distVs = [&](std::pair<int,int> pos1, std::pair<int,int> pos2){
        int cont = 0;
        unordered_map<Vehiculo*,Vehiculo*> table;//tan sólo necesitamos comprobar que no se repitan punteros (también se podrían obtener las letras)
        
        int posicion1A, posicion1B, posicion2;

        if(pos1.first == pos2.first){
            posicion1A = pos1.second, posicion1B = pos1.first, posicion2 = pos2.second;
        }else
            posicion1A = pos1.first, posicion1B = pos1.second, posicion2 = pos2.first;

        int end = std::max(posicion1A, posicion2);
        for(int i = std::min(posicion1A, posicion2); i < end; i++){
            if(table.count(matriz[posicion1B][i]) == 0){
                cont++;
                table.insert({matriz[posicion1B][i],matriz[posicion1B][i]});
            }
        }
        return cont;
    };
    
    hX = distVs(posX, {posZ.first, posX.second}) + distVs(posZ, {posZ.first, posX.second});
    
    //cual borde es el más cercano?
    int borderDistsZ[4] {abs(posZ.first - ancho), posZ.first, posZ.second, abs(posZ.second - alto)};
    int mini = 0;
    for(int i = 1; i < 4; i++)
        if(borderDistsZ[i] < borderDistsZ[mini])
            mini = i;
    switch(mini){
    case 0://derecho
        hZ = distVs(posZ, {ancho-1, posZ.second});
        break;
    case 1://izquierdo
        hZ = distVs(posZ, {0, posZ.second});
        break;
    case 2://arriba
        hZ = distVs(posZ, {posZ.first, 0});
        break;
    case 3://abajo
        hZ = distVs(posZ, {posZ.first, alto-1});
        break;
    }
    //std::cout << "hX: " << hX << " | hZ: " << hZ << endl;
    return hZ + hX;
}

vector<pair<Vehiculo*, Tablero::direccion>> Tablero::movimientosPosibles(){
    vector<pair<Vehiculo*, direccion>> sol;

    if(posX.first > 0){ // Izquierda
        if(matriz[posX.first-1][posX.second]->horizontal){
            sol.push_back(pair(matriz[posX.first-1][posX.second], IZQUIERDA));
        }
    }

    if(posX.second > 0){ // Abajo
        if(matriz[posX.first][posX.second+1]->vertical){
            sol.push_back(pair(matriz[posX.first][posX.second+1], ABAJO));
        }
    }

    if(posX.first < alto - 1){ // Arriba
        if(matriz[posX.first][posX.second-1]->vertical){
            sol.push_back(pair(matriz[posX.first][posX.second-1], ARRIBA));
        }
    }

    if(posX.first < ancho - 1){ // Derecha
        if(matriz[posX.first+1][posX.second]->horizontal){
            sol.push_back(pair(matriz[posX.first+1][posX.second], DERECHA));
        }
    }

    return sol;
}

void Tablero::realizarMovimiento(pair<Vehiculo*, Tablero::direccion> v){
    Vehiculo* h;
    
    matriz[posX.first][posX.second] = v.first;//movemos en el tablero
    switch(v.second){
    case IZQUIERDA:
    case ARRIBA:
        v.first->partes.push_back(posX);//movemos la parte quitada al otro extremo
        posX = v.first->partes.front();//el hueco pasa al extremo contrario del vehiculo
        v.first->partes.pop_front();//quitamos un extremo del vehiculo
        break;
    case DERECHA:
    case ABAJO:
        v.first->partes.push_front(posX);
        posX = v.first->partes.back();
        v.first->partes.pop_back();
        break;
    }
    if(v.first->letra == 'Z')
        posZ = v.first->partes.front();
}

Tablero::~Tablero(){
    for(auto it = vehiculos.begin(); it != vehiculos.end(); it++)
        delete *it;
}
