#include "vehiculo.h"

Vehiculo::Vehiculo() {}

Vehiculo::Vehiculo(bool vertical, std::initializer_list<std::pair<int,int>> &l)
    : vertical(vertical), partes(l), longitud(l.size()) { }
