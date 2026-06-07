#ifndef MOTOR_BUSCA_HPP
#define MOTOR_BUSCA_HPP

#include <unordered_set>
#include "Estruturas.hpp"
#include <vector>

class MotorBusca
{
public:
    // A função que testa a vitória
    bool isGoal(const std::vector<unsigned int> &estado_atual, int tamanho_grid);
    // A função que gera as próximas jogadas válidas
    std::vector<State *> getNeighbors(State *estado_atual, int tamanho_grid);

    bool isSolvable(std::vector<unsigned int> estado, int tamanho_grid);
    void executaA_estrela(Instancia &instancia, int tamanho_grid);

    // O algoritmo principal
    void executarDijkstra(const Instancia &instancia, int tamanho_grid);
    void preencherMatrizDistancia(Instancia &instancia, int tamanho_grid);
    // IDA*
    int recursiveSearch(State *current_state, int limite, int cost, std::unordered_set<std::vector<unsigned int>, HashFunction> &path_state,
                        int &iterations, std::vector<std::vector<unsigned int>> &distance, int tamanho_grid, std::vector<std::vector<unsigned int>> &current_path);

    void executaIDA_estrela(Instancia &instancia, int tamanho_grid);

    void imprimirCaminho(State *objetivo, int tamanho_grid);
    unsigned int findZero(const Instancia &instancia);
};

#endif