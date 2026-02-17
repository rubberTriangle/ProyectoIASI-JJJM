#ifndef VEHICULO_H
#define VEHICULO_H

#include <list>

//PRE: todas las partes del vehiculo están en línea y tiene al menos 1
struct Vehiculo
{
    bool vertical, horizontal;
    std::list<std::pair<int,int>> partes;//orden: desde izquierd/arriba a derecha/abajo
    int longitud;
    char letra;
    
    //averigua su direccion y longitud en base a partes y guarda el resultado en este objeto
    void calcularDesdePartes();
    
    Vehiculo(char letra);
    Vehiculo(char letra, bool vertical, bool horizontal, std::initializer_list<std::pair<int,int>> &l);
};

#endif // VEHICULO_H
