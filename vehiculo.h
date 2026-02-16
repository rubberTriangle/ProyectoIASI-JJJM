#ifndef VEHICULO_H
#define VEHICULO_H

#include <vector>

//PRE: todas las partes del vehiculo están en línea y tiene al menos 2
class Vehiculo
{
public://miembros expuestos para evitar llamadas innecesarias
    bool vertical, horizontal;
    std::vector<std::pair<int,int>> partes;
    int longitud;
    char letra;
    
    //averigua su direccion y longitud en base a partes y guarda el resultado en este objeto
    void calcularDesdePartes();
    
    Vehiculo(char letra);
    Vehiculo(char letra, bool vertical, bool horizontal, std::initializer_list<std::pair<int,int>> &l);
};

#endif // VEHICULO_H
