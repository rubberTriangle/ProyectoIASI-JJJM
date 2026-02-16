#include "Tablero.h"
#include <iostream>
#include <unordered_map>

Tablero::Tablero() {
    cout << "Introduce el tablero que quieres utilizar: " << endl;
    cin >> nombreTablero;

    cout << "Introduce la anchura y la altura del tablero del fichero seleccionado: " << endl;
    cin >> ancho >> alto;

    importarMatriz();
}

void Tablero::procesarMatriz(vector<vector<char>> const& chmat, vector<vector<Vehiculo*>> & m){
    std::unordered_map<char, Vehiculo*> vmap;
    m.assign(chmat.size(), std::vector<Vehiculo*>(chmat[0].size()));
    for(int i = 0; i < chmat.size(); i++)
        for(int j = 0; j < chmat[0].size(); j++){
            char current = chmat[i][j];
            Vehiculo* cv;
            if(current == 'X')
                posX = {i, j};
            else{
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
            cout << (j==posX.first && i==posX.second ? 'X' : matriz[j][i]->letra) << " ";
        }
        cout << endl;
    }
}

vector<pair<Vehiculo, Tablero::direccion>> Tablero::movimientosPosibles()
{
    vector<pair<Vehiculo, direccion>> sol;

    if(posX.first > 0){ // Izquierda
        if(matriz[posX.first-1][posX.second]->horizontal){
            sol.push_back(pair(*matriz[posX.first-1][posX.second], IZQUIERDA));
        }
    }

    if(posX.second > 0){ // Abajo
        if(matriz[posX.first][posX.second+1]->vertical){
            sol.push_back(pair(*matriz[posX.first][posX.second+1], ABAJO));
        }
    }

    if(posX.first < alto - 1){ // Arriba
        if(matriz[posX.first][posX.second-1]->vertical){
            sol.push_back(pair(*matriz[posX.first][posX.second-1], ARRIBA));
        }
    }

    if(posX.first < ancho - 1){ // Derecha
        if(matriz[posX.first+1][posX.second]->horizontal){
            sol.push_back(pair(*matriz[posX.first+1][posX.second], DERECHA));
        }
    }

    return sol;
}

void Tablero::realizarMovimiento(pair<Vehiculo, Tablero::direccion> v){
    Vehiculo* h;
    std::pair<int,int> oldPosX = posX;
    switch(v.second){
    case IZQUIERDA:
        h = matriz[oldPosX.first-1][oldPosX.second];
        posX = h->partes.front();//el hueco pasa al extremo contrario del vehiculo
        h->partes.pop_front();//quitamos un extremo del vehiculo
        break;
    case ARRIBA:
        h = matriz[oldPosX.first][oldPosX.second+1];
        posX = h->partes.front();//el hueco pasa al extremo contrario del vehiculo
        h->partes.pop_front();//quitamos un extremo del vehiculo
        break;
    case DERECHA:
        h = matriz[oldPosX.first+1][oldPosX.second];
        posX = h->partes.back();//el hueco pasa al extremo contrario del vehiculo
        h->partes.pop_back();//quitamos un extremo del vehiculo
        break;
    case ABAJO:
        h = matriz[oldPosX.first][oldPosX.second-1];
        posX = h->partes.back();//el hueco pasa al extremo contrario del vehiculo
        h->partes.pop_back();//quitamos un extremo del vehiculo
        break;
    }
    matriz[oldPosX.first][oldPosX.second] = h;//movemos en el tablero
    h->partes.push_back(oldPosX);//movemos la parte quitada al otro extremo
}

Tablero::~Tablero(){
    for(auto it = vehiculos.begin(); it != vehiculos.end(); it++)
        delete *it;
}
