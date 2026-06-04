#include "../Bibliotecas/MotorBusca.hpp"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iostream>

bool MotorBusca::isGoal(const std::vector<unsigned int> &estado_atual, int grid_size)
{
    unsigned int size = grid_size * grid_size;

    // espaco vazio = primeira peca
    if (estado_atual[0] != 0)
        return false;

    // demais pecas em seus indices respectivos
    for (unsigned int i = 1; i < size; i++)
    {
        if (estado_atual[i] != i)
            return false;
    }

    return true;
}

// std::vector<unsigned int> copy(State *estado_atual)
// {
//     std::vector<unsigned int> temp;
//     for (size_t i = 0; i < estado_atual->estado.size(); i++)
//     {
//         temp.push_back(estado_atual->estado[i]);
//     }
//     return temp;
// }

std::vector<State *> MotorBusca::getNeighbors(State *estado_atual, int tamanho_grid)
{
    std::vector<State *> neighbors;
    std::vector<unsigned int> vetor_atual = estado_atual->estado;
    int pos_vazio = estado_atual->pos_vazio;
    int total_pecas = tamanho_grid * tamanho_grid;

    if (pos_vazio >= tamanho_grid)
    {
        // std::vector<unsigned int> newVector = copy(estado_atual);
        std::vector<unsigned int> newVector = vetor_atual;
        std::swap(newVector[pos_vazio], newVector[pos_vazio - tamanho_grid]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio - tamanho_grid, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if (pos_vazio < (total_pecas - tamanho_grid))
    {
        // std::vector<unsigned int> newVector = copy(estado_atual);
        std::vector<unsigned int> newVector = vetor_atual;
        std::swap(newVector[pos_vazio], newVector[pos_vazio + tamanho_grid]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio + tamanho_grid, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if (pos_vazio % tamanho_grid != 0)
    {
        // std::vector<unsigned int> newVector = copy(estado_atual);
        std::vector<unsigned int> newVector = vetor_atual;
        std::swap(newVector[pos_vazio], newVector[pos_vazio - 1]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio - 1, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if ((pos_vazio + 1) % tamanho_grid != 0)
    {
        // std::vector<unsigned int> newVector = copy(estado_atual);
        std::vector<unsigned int> newVector = vetor_atual;
        std::swap(newVector[pos_vazio], newVector[pos_vazio + 1]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio + 1, 0, estado_atual);
        neighbors.push_back(newState);
    }

    return neighbors;
}

unsigned int MotorBusca::findZero(const Instancia &instancia)
{
    for (size_t i = 0; i < instancia.tabuleiro.size(); i++)
    {
        if (instancia.tabuleiro[i] == 0)
            return i;
    }
    return 0;
}

void printState(const std::vector<unsigned int> &state)
{
    for (size_t i = 0; i < state.size(); i++)
    {
        std::cout << state[i];
    }
    std::cout << std::endl;
}

// distancia de Manhattan
int MotorBusca::calcula_heuristica(const std::vector<unsigned int> &estado, const std::vector<std::vector<unsigned int>> &matriz_distancia)
{
    int h = 0;

    for (size_t pos = 0; pos < estado.size(); pos++)
    {
        unsigned int peca = estado[pos];

        if (peca != 0)
        {
            h += matriz_distancia[peca][pos];
        }
    }
    return h;
}

void MotorBusca::imprimirCaminho(State *objetivo, int tamanho_grid)
{
    std::vector<State *> caminho;
    State *atual = objetivo;

    while (atual != nullptr)
    {
        caminho.push_back(atual);
        atual = atual->parent;
    }

    std::reverse(caminho.begin(), caminho.end());

    std::cout << "Solucao encontrada em " << caminho.size() - 1 << "movimentos" << std::endl;

    for (size_t i = 0; i < caminho.size(); i++)
    {
        std::cout << "Passo " << i << "(Custo G: " << caminho[i]->custo
                  << ", Heuristica H: " << caminho[i]->custo_h << "):" << std::endl;
        for (size_t j = 0; j < caminho[i]->estado.size(); j++)
        {
            if (caminho[i]->estado[j] == 0)
                std::cout << std::setw(2) << " " << " ";
            else
                std::cout << std::setw(2) << caminho[i]->estado[j] << " ";
            if ((j + 1) % tamanho_grid == 0)
                std::cout << std::endl;
        }
        std::cout << "------------" << std::endl;
    }
}

void MotorBusca::executaA_estrela(Instancia &instancia, int tamanho_grid)
{
    preencherMatrizDistancia(instancia, tamanho_grid);

    std::priority_queue<State *, std::vector<State *>, Comparador> queue;
    std::unordered_set<std::vector<unsigned int>, HashFunction> closedSet;

    int pos_zero = findZero(instancia);

    int h_inicial = calcula_heuristica(instancia.tabuleiro, instancia.matriz_distancia);

    State *initial_state = new State(instancia.tabuleiro, 0, pos_zero, h_inicial, nullptr);

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
            // std::cout << current->custo << std::endl;
            // printState(current->estado);
            imprimirCaminho(current, tamanho_grid);
            break;
        }

        std::vector<State *> neighbors = getNeighbors(current, tamanho_grid);

        for (size_t i = 0; i < neighbors.size(); i++)
        {
            if (!closedSet.count(neighbors[i]->estado))
            {
                neighbors[i]->custo_h = calcula_heuristica(neighbors[i]->estado, instancia.matriz_distancia);
                queue.push(neighbors[i]);
            }
            else
            {
                delete neighbors[i];
            }
        }
    }
    if (queue.empty())
    {
        std::cout << "Fila vazia! Nenhuma solucao foi encontrada. O mapa foi todo explorado." << std::endl;
    }
}
// int recursiveSearch(std::vector<unsigned int> estado, int limite, int g, std::vector<std::vector<unsigned int>> current_path,
//                                 int &iteracoes)
// {
//     int h = calcula_heuristica();
// }

// void MotorBusca::executaIDA_estrela(const Instancia &instancia, int tamanho_grid)
// {
//     int limite = calcula_heuristica();
//     std::vector<std::vector<unsigned int>> current_path;
//     current_path.push_back(instancia.tabuleiro);
//     int result = 0;
//     int iteracoes = 0;

//     while (true)
//     {
//         result = recursiveSearch(instancia.tabuleiro, limite, 0, current_path, iteracoes);

//         if (result == 0)
//         {
//             std::cout << "Resultado Encontrado" << std::endl;
//             std::cout << "Quantidade de Iteracoes" << std::endl;
//         }

//         else if (result == -1)
//         {
//             std::cout << "Este tabuleiro não possui solução" << std::endl;
//             return;
//         }

//         limite = result;
//     }
//     return;
// }

void MotorBusca::preencherMatrizDistancia(Instancia &instancia, int tamanho_grid)
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