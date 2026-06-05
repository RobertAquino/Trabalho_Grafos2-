#ifndef ESTRUTURAS
#define ESTRUTURAS
#include <iostream>
#include <vector>
class Instancia
{
public:
    std::vector<unsigned int> tabuleiro;
    std::vector<std::vector<unsigned int>> matriz_distancia;

    void preencherMatrizDistancia(Instancia &instancia, int tamanho_grid)
    {
        int num_pecas = tamanho_grid * tamanho_grid;

        // redimensiona a matriz para [peca][posicao_atual]
        instancia.matriz_distancia.assign(num_pecas, std::vector<unsigned int>(num_pecas, 0));

        for (int peca = 1; peca < num_pecas; peca++)
        {
            // enconta o local correto da peca
            int target_idx = peca;
            int target_row = target_idx / tamanho_grid;
            int target_col = target_idx % tamanho_grid;

            for (int pos = 0; pos < num_pecas; pos++)
            {
                int current_row = pos / tamanho_grid;
                int current_col = pos % tamanho_grid;

                // fórmula - Distancia de Manhattan: |x1 - x2| + |y1 - y2|
                int dist = std::abs(current_row - target_row) + std::abs(current_col - target_col);

                // salva na matriz
                instancia.matriz_distancia[peca][pos] = dist;
            }
        }
    }
};

class State
{
public:
    std::vector<unsigned int> estado;
    double custo;
    int pos_vazio;
    int custo_h;
    State *parent;

public:
    State(std::vector<unsigned int> est, double c, int pos, int h, State *p) : estado(est), custo(c), pos_vazio(pos), custo_h(h), parent(p) {}
    double getF() const
    {
        return custo + custo_h;
    }
};

class HashFunction
{
public:
    size_t operator()(const std::vector<unsigned int> &vector) const
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
public:
    size_t operator()(const State *a, const State *b) const
    {
        return a->getF() > b->getF();
    }
};
#endif