#include "escaladasimpleestocastica.h"

escaladaSimpleEstocastica::escaladaSimpleEstocastica() {
    cout << "Método elegido: escalada simple estocástica" << endl;
    cout << "Introducir valor de alfa (Pulsar enter para valores predeterminados" << endl;
    getline(std::cin, alfa);
    if (alfa.empty()) {
        alfa = 3;
    }
    cout << "Alfa = " << alfa << endl;
}

void escaladaSimpleEstocastica::mover() {

}
