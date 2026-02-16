#ifndef VEHICULO_H
#define VEHICULO_H

#include <vector>


class Vehiculo
{
public:
    bool vertical;
    std::vector<std::pair<int,int>> partes;
    
    Vehiculo();
};

#endif // VEHICULO_H
