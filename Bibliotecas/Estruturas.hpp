#ifndef ESTRUTURAS
#define ESTRUTURAS
#include <iostream>
#include <vector>
class Instancia
{
public:
    std::vector<unsigned int> tabuleiro;
    std::vector<std::vector<unsigned int>> matriz_distancia;
};

class State
{
public:
    std::vector<unsigned int> estado;
    State *parent;
    double custo;
    int pos_vazio;
    int custo_h;

public:
    double getF() const
    {
        return custo + custo_h;
    }
};

#endif