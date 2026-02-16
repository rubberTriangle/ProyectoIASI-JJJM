#include "vehiculo.h"

void Vehiculo::calcularDesdePartes(){
    int dh, dv;
    dh = partes.back().first - partes.front().first;
    dv = partes.back().second - partes.front().second;
    if(dv != 0){
        vertical = true;
        longitud = dv+1;
    }else if(dh != 0){
        horizontal = true;
        longitud = dh+1;
    }else{
        vertical = true;
        horizontal = true;
        longitud = 1;
    }
    longitud = std::max(dv, dh);
}

Vehiculo::Vehiculo(char letra) : vertical(false), horizontal(false), letra(letra) {}

Vehiculo::Vehiculo(char letra, bool vertical, bool horizontal, std::initializer_list<std::pair<int,int>> &l)
    : letra(letra), vertical(vertical), horizontal(horizontal), partes(l), longitud(l.size()) { }
