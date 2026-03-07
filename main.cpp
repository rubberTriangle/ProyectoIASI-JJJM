#include <iostream>
#include <climits>

#include "Tablero.h"
#include "jugar.h"

using namespace std;

int main() {
    jugar jugar;
    jugar.elegirHeuristica ();
    jugar.elegirMetodo ();

    return 0;
}
