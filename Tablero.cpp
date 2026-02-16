#include "Tablero.h"
#include <iostream>

Tablero::Tablero() {
    cout << "Introduce el tablero que quieres utilizar: " << endl;
    cin >> nombreTablero;

    cout << "Introduce la anchura y la altura del tablero del fichero seleccionado: " << endl;
    cin >> ancho >> alto;
}

void Tablero::procesarMatriz(vector<vector<char>> const& chmat, vector<vector<Vehiculo*>> & m){
    
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
