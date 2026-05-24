#ifndef MOTOR_BUSCA_HPP
#define MOTOR_BUSCA_HPP

#include "Estruturas.hpp"
#include <vector>

class MotorBusca {
public:

    // A função que testa a vitória
    bool isGoal(const std::vector<unsigned int>& estado_atual, int tamanho_grid);
    
    // A função que gera as próximas jogadas válidas
    std::vector<State*> getNeighbors(State* estado_atual, int tamanho_grid);
    
    // O algoritmo principal
    void executarDijkstra(const Instancia& instancia, int tamanho_grid);
};

#endif