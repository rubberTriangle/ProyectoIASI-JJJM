#ifndef TABLERO_H
#define TABLERO_H

class tablero
{
private:
    int ancho, alto;
    string nombreTablero;
public:
    char matriz[ancho][alto];
    tablero();
    void importarMatriz(string nombreTablero);
};

#endif // TABLERO_H
