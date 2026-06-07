#include "../Bibliotecas/MotorBusca.hpp"
#include "../Bibliotecas/Heuristicas.hpp"
#include <queue>
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
bool MotorBusca::isSolvable(std::vector<unsigned int> estado, int tamanho_grid)
{
    int inversoes = 0;
    int size = estado.size();
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
            if (estado[i] != 0 && estado[j] != 0 && estado[j] < estado[i])
                inversoes++;
    }

    if (tamanho_grid % 2 != 0)
        return inversoes % 2 == 0;

    int zero_line = 0;
    for (int i = 0; i < size; i++)
    {
        if (estado[i] == 0)
            zero_line = tamanho_grid - (i / tamanho_grid);
    }

    std::cout << "Inversoes: " << inversoes << " | Linha do zero: " << zero_line << std::endl;
    if (zero_line % 2 == 0)
        return inversoes % 2 == 0;

    return inversoes % 2 != 0;
}
std::vector<State *> MotorBusca::getNeighbors(State *estado_atual, int tamanho_grid)
{
    std::vector<State *> neighbors;
    std::vector<unsigned int> vetor_atual = estado_atual->estado;
    int pos_vazio = estado_atual->pos_vazio;
    int total_pecas = tamanho_grid * tamanho_grid;

    if (pos_vazio >= tamanho_grid)
    {
        std::vector<unsigned int> newVector = vetor_atual;
        std::swap(newVector[pos_vazio], newVector[pos_vazio - tamanho_grid]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio - tamanho_grid, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if (pos_vazio < (total_pecas - tamanho_grid))
    {
        std::vector<unsigned int> newVector = vetor_atual;
        std::swap(newVector[pos_vazio], newVector[pos_vazio + tamanho_grid]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio + tamanho_grid, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if (pos_vazio % tamanho_grid != 0)
    {
        std::vector<unsigned int> newVector = vetor_atual;
        std::swap(newVector[pos_vazio], newVector[pos_vazio - 1]);

        State *newState = new State(newVector, estado_atual->custo + 1, pos_vazio - 1, 0, estado_atual);
        neighbors.push_back(newState);
    }

    if ((pos_vazio + 1) % tamanho_grid != 0)
    {
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
void MotorBusca::executaA_estrela(Instancia &instancia, int tamanho_grid)
{
    preencherMatrizDistancia(instancia, tamanho_grid);
    if (!isSolvable(instancia.tabuleiro, tamanho_grid))
    {
        std::cout << "Este tabuleiro e insolúvel" << std::endl;
        return;
    }

    std::cout << "Esta instancia e solucionável" << std::endl;

    std::priority_queue<State *, std::vector<State *>, Comparador> queue;
    std::unordered_set<std::vector<unsigned int>, HashFunction> closedSet;

    int pos_zero = findZero(instancia);

    int h_manhattan_ini = calcula_heuristica(instancia.tabuleiro, instancia.matriz_distancia);
    int h_gaschnig_ini = calcula_gaschnig(instancia.tabuleiro);
    int h_inicial = std::max(h_manhattan_ini, h_gaschnig_ini);

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
            imprimirCaminho(current, tamanho_grid);

            break;
        }

        std::vector<State *> neighbors = getNeighbors(current, tamanho_grid);

        for (size_t i = 0; i < neighbors.size(); i++)
        {
            if (!closedSet.count(neighbors[i]->estado))
            {
                int h_manhattan = calcula_heuristica(neighbors[i]->estado, instancia.matriz_distancia);
                int h_gaschnig = calcula_gaschnig(neighbors[i]->estado);
                int h = std::max(h_manhattan, h_gaschnig);
                neighbors[i]->custo_h = h;
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
int MotorBusca::recursiveSearch(State *current_state, int limite, int cost, std::unordered_set<std::vector<unsigned int>, HashFunction> &path_state,
                                int &iterations, std::vector<std::vector<unsigned int>> &distance, int tamanho_grid, std::vector<std::vector<unsigned int>> &current_path)
{
    int h_manhattan = calcula_heuristica(current_state->estado, distance);
    int h_gaschnig = calcula_gaschnig(current_state->estado);
    int h = std::max(h_manhattan, h_gaschnig);
    int total_cost = h + cost;

    // oi
    if (total_cost > limite)
        return total_cost;

    if (isGoal(current_state->estado, tamanho_grid))
    {
        imprimirCaminho(current_state, tamanho_grid);
        return 0;
    }

    std::vector<State *> neighbors = getNeighbors(current_state, tamanho_grid);

    int lower_cost = std::numeric_limits<int>::max();
    ;
    for (size_t i = 0; i < neighbors.size(); i++)
    {
        iterations++;
        if (path_state.count(neighbors[i]->estado) == 0)
        {
            path_state.insert(neighbors[i]->estado);
            current_path.push_back(neighbors[i]->estado);
            neighbors[i]->custo_h = h;

            int result_son = recursiveSearch(neighbors[i], limite, cost + 1, path_state, iterations, distance, tamanho_grid, current_path);

            path_state.erase(neighbors[i]->estado);
            current_path.pop_back();
            delete neighbors[i];

            if (result_son == 0)
            {
                for (size_t j = i + 1; j < neighbors.size(); j++)
                {
                    delete neighbors[j];
                }
                return 0;
            }

            lower_cost = std::min(lower_cost, result_son); // acumula
        }
        else
        {
            delete neighbors[i];
        }
    }
    return lower_cost;
}

void MotorBusca::executaIDA_estrela(Instancia &instancia, int tamanho_grid)
{
    preencherMatrizDistancia(instancia, tamanho_grid);

    if (!isSolvable(instancia.tabuleiro, tamanho_grid))
    {
        std::cout << "Este tabuleiro e insolúvel" << std::endl;
        return;
    }

    std::cout << "Esta instancia e solucionável" << std::endl;

    int h_manhattan_ini = calcula_heuristica(instancia.tabuleiro, instancia.matriz_distancia);
    int h_gaschnig_ini = calcula_gaschnig(instancia.tabuleiro);
    int limite = std::max(h_manhattan_ini, h_gaschnig_ini);

    std::unordered_set<std::vector<unsigned int>, HashFunction> path_state;
    std::vector<std::vector<unsigned int>> current_path;

    int result = 1;
    int iterations = 0;
    std::vector<std::vector<unsigned int>> distancia = instancia.matriz_distancia;
    unsigned int pos_vazio = findZero(instancia);
    State *current_state = new State(instancia.tabuleiro, 0, pos_vazio, limite, nullptr);

    while (true)
    {
        path_state.clear();
        current_path.clear();

        path_state.insert(instancia.tabuleiro);
        current_path.push_back(instancia.tabuleiro);
        result = recursiveSearch(current_state, limite, 0, path_state, iterations, distancia, tamanho_grid, current_path);

        if (result == 0)
        {
            delete current_state;
            break;
        }

        limite = result;
    }
    return;
}