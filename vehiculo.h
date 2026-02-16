#ifndef VEHICULO_H
#define VEHICULO_H

#include <vector>

class Vehiculo
{
public://miembros expuestos para evitar llamadas innecesarias
    bool vertical;
    std::vector<std::pair<int,int>> partes;
    int longitud;
    
    Vehiculo();
    Vehiculo(bool vertical, std::initializer_list<std::pair<int,int>> &l);
};

#endif // VEHICULO_H
