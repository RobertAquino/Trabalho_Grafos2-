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
    State(std::vector<unsigned int> est, double c, int pos, int h, State *p) : estado(est), custo(c), pos_vazio(pos), custo_h(h), parent(p) {}
    double getF() const
    {
        return custo + custo_h;
    }
};

class HashFunction
{
    size_t operator()(const std::vector<unsigned int> &vector)
    {
        size_t hash = 0;

        for (const auto value : vector)
        {
            hash = hash * 31 + value;
        }

        return hash;
    }
};

class Comparador
{
    size_t operator()(const State *a, const State *b)
    {
        return a->getF() > b->getF();
    }
};
#endif