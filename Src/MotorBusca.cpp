#include "../Bibliotecas/MotorBusca.hpp"
#include <queue>
#include <unordered_set>

bool isGoal(const std::vector<unsigned int> &estado_atual, int grid_size)
{
    int size = grid_size * grid_size;

    for (int i = 0; i < size; i++)
    {
        if (estado_atual[i] != i + 1)
            return false;
    }

    if (estado_atual[grid_size - 1] != 0)
        return false;

    return true;
}

std::vector<unsigned int> copy(State *estado_atual)
{
    std::vector<unsigned int> temp;
    for (size_t i = 0; i < estado_atual->estado.size(); i++)
    {
        temp.push_back(estado_atual->estado[i]);
    }
    return temp;
}

std::vector<State *> getNeighbors(State *estado_atual, int tamanho_grid)
{
    std::vector<State *> neighbors;
    std::vector<unsigned int> estado_atual = estado_atual->estado;
    int pos_vazio = estado_atual->pos_vazio;
    int total_pecas = tamanho_grid * tamanho_grid;

    if (pos_vazio > tamanho_grid)
    {
        std::vector<unsigned int> newVector = copy(estado_atual);
        std::swap(newVector[pos_vazio], newVector[pos_vazio - tamanho_grid]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio - tamanho_grid, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if (pos_vazio < (total_pecas - tamanho_grid))
    {
        std::vector<unsigned int> newVector = copy(estado_atual);
        std::swap(newVector[pos_vazio], newVector[pos_vazio + tamanho_grid]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio + tamanho_grid, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if (pos_vazio % tamanho_grid != 0)
    {
        std::vector<unsigned int> newVector = copy(estado_atual);
        std::swap(newVector[pos_vazio], newVector[pos_vazio - 1]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio - 1, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if ((pos_vazio + 1) % tamanho_grid != 0)
    {
        std::vector<unsigned int> newVector = copy(estado_atual);
        std::swap(newVector[pos_vazio], newVector[pos_vazio + 1]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio + 1, 0, estado_atual);
        neighbors.push_back(newState);
    }

    return neighbors;
}
unsigned int findZero(const Instancia &instancia)
{
    for (size_t i = 0; i < instancia.tabuleiro.size(); i++)
    {
        if (instancia.tabuleiro[i] == 0)
            return i;
    }
}

void printState(const std::vector<unsigned int> &state)
{
    for (size_t i = 0; i < state.size(); i++)
    {
        std::cout << state[i];
    }
    std::cout << std::endl;
}
// temos que criar a heurística
int calcula_heuristica()
{
    return 0;
}
void executaA_estrela(const Instancia &instancia, int tamanho_grid)
{
    std::priority_queue<State *, std::vector<State *>, Comparador> queue;
    std::unordered_set<std::vector<unsigned int>, HashFunction> closedSet;

    int heuristica = calcula_heuristica(); // não temos heurística por enquanto

    int pos_zero = findZero(instancia);
    State *initial_state = new State(instancia.tabuleiro, 0, pos_zero, 0, nullptr);

    queue.push(initial_state);

    while (!queue.empty())
    {
        State *current = queue.top();
        queue.pop();

        if (closedSet.count(current->estado) > 0)
        {
            delete current;
            continue;
        }

        closedSet.insert(current->estado);

        if (isGoal(current->estado, tamanho_grid))
        {
            std::cout << current->custo << std::endl;
            printState(current->estado);
            break;
        }

        std::vector<State *> neighbors = getNeighbors(current, tamanho_grid);

        for (size_t i = 0; i < neighbors.size(); i++)
        {
            if (!closedSet.count(neighbors[i]->estado))
            {
                neighbors[i]->custo_h = calcula_heuristica();
                queue.push(neighbors[i]);
            }
            else
            {
                delete neighbors[i];
            }
        }
    }
}
void executaIDA_estrela()
{
    return;
}