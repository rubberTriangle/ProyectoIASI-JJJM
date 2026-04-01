#ifndef BUSQUEDAPRIMEROMEJORAC_H
#define BUSQUEDAPRIMEROMEJORAC_H

#include <queue>

class busquedaPrimeroMejorAC {
private:
    priority_queue abiertos;
    queue cerrados;
public:
    busquedaPrimeroMejorAC();
    void mover();
};

#endif // BUSQUEDAPRIMEROMEJORAC_H
