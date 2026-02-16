#include "tablero.h"

tablero::tablero() {
    cout << "Introduce el tablero que quieres utilizar: " << endl;
    cin >> nombreTablero;
}

void tablero::importaMatriz(){
    cout << "Introduce la anchura y la altura del tablero del fichero seleccionado: " << endl;
    cin >> ancho >> alto;

    ifstream archivo("ficheros/" + nombreTablero + ".txt");
    string linea;

    if(archivo.is_open()){
        int j = 0;
        while(getline(archivo, linea)){
            int k = 0;
            for(int i = 0; i < linea.size(); i++){
                if(linea[i] != ','){
                    matriz[j][k] = linea[i];
                    k++;
                }
            }
            j++;
        }

        archivo.close();
    }else{
        cerr << "No se ha abierto el fichero correctamente";
    }
}

void mostrarMatriz(){
    for(int j = 0; j < alto; j++){
        for(int i = 0; i < ancho; i++){
            cout << matriz[j][i] << " ";
        }
        cout << endl;
    }
}
