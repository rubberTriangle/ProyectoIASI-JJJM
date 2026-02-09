#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    int ancho, alto;
    cin >> ancho >> alto;

    ifstream archivo("ficheros/HORAPUNTA1.txt");
    string linea;
    char matriz[ancho][alto];

    if(archivo.is_open()){
        int j = 0;
        while(getline(archivo, linea)){
            cout << linea << endl;
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

        for(int j = 0; j < alto; j++){
            for(int i = 0; i < ancho; i++){
                cout << matriz[j][i] << " ";
            }
            cout << endl;
        }



    }else{
        cerr << "No se ha abierto el fichero correctamente";
    }

    return 0;
}
