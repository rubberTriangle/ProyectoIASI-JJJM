#include "Tablero.h"
#include <iostream>
#include <unordered_map>

Tablero::Tablero() {
    cout << "Introduce el tablero que quieres utilizar: " << endl;
    cin >> nombreTablero;

    cout << "Introduce la anchura y la altura del tablero del fichero seleccionado: " << endl;
    cin >> ancho >> alto;
}

void Tablero::procesarMatriz(vector<vector<char>> const& chmat, vector<vector<Vehiculo*>> & m){
    std::unordered_map<char, Vehiculo*> vmap;
    m.assign(chmat.size(), std::vector<Vehiculo*>(chmat[0].size()));
    for(int i = 0; i < chmat.size(); i++)
        for(int j = 0; j < chmat[0].size(); j++){
            char current = chmat[i][j];
            Vehiculo* cv;
            if(vmap.count(current) == 0){//no está el vehiculo
                cv = new Vehiculo();
                vehiculos.push_back(cv);
                vmap.insert({current, cv});
            }else{
                cv = (*vmap.find(current)).second;
            }
            cv->partes.push_back({i,j});
            m[i][j] = cv;
        }
    
    for(auto it = vehiculos.begin(); it != vehiculos.end(); it++){
        int lh, lv;
        lh = (*it)->partes.back().first - (*it)->partes.front().first;
        lv = (*it)->partes.back().second - (*it)->partes.front().second;
        (*it)->vertical = (lv != 0);
        (*it)->longitud = std::max(lv, lh);
    }
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
            cout << matriz[j][i] << " ";
        }
        cout << endl;
    }
}

Tablero::~Tablero(){
    for(auto it = vehiculos.begin(); it != vehiculos.end(); it++)
        delete *it;
}
