#ifndef MOTOR_BUSCA_HPP
#define MOTOR_BUSCA_HPP

#include "Estruturas.hpp"
#include <vector>

class MotorBusca
{
public:
    // A função que testa a vitória
    bool isGoal(const std::vector<unsigned int> &estado_atual, int tamanho_grid);
    // A função que gera as próximas jogadas válidas
    std::vector<State *> getNeighbors(State *estado_atual, int tamanho_grid);
    void executaA_estrela(Instancia &instancia, int tamanho_grid);

    // O algoritmo principal
    // void executarDijkstra(const Instancia &instancia, int tamanho_grid);

    // IDA*
    // int recursiveSearch(State *current, int limite, Instancia &instancia, int tamanho_grid, std::unordered_set<std::vector<unsigned int>, HashFunction> &path_set, int &iteracoes);
    // void executaIDA_estrela(const Instancia &instancia, int tamanho_grid);

    void preencherMatrizDistancia(Instancia &instancia, int tamanho_grid);

    // distância de Manhattan
    int calcula_heuristica(const std::vector<unsigned int> &estado, const std::vector<std::vector<unsigned int>> &matriz_distancia);
    void imprimirCaminho(State *objetivo, int tamanho_grid);
    unsigned int findZero(const Instancia &instancia);

    // heurística de Gaschnig
    int calcula_gaschnig(const std::vector<unsigned int> &estado);
};

#endif