#include "../Bibliotecas/MotorBusca.hpp"

bool isGoal(const std::vector<unsigned int> &estado_atual, int tamanho_grid)
{
    int tamanho_total = tamanho_grid * tamanho_grid;

    for (int i = 0; i < tamanho_total; i++)
    {
        if (estado_atual[i] != i + 1)
            return false;
    }

    if (estado_atual[tamanho_total - 1] != 0)
        return false;

    return true;
}

std::vector<State *> getNeighbors(State *estado_atual, int tamanho_grid)
{
    std::vector<State *> vizinhos;
    std::vector<unsigned int> estado_atual = estado_atual->estado;
    int pos_vazio = estado_atual->pos_vazio;
    int total_peças = tamanho_grid * tamanho_grid;
}

void executarDijkstra(const Instancia &instancia, int tamanho_grid);