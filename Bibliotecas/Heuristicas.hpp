#ifndef HEURISTICAS_HPP
#define HEURISTICAS_HPP

#include <vector>
#include <unordered_map>
#include <queue>

struct StateMatrix
{
    int matrix[5][5];
    int pos_zero;
    int custo;
};

int calcula_heuristica(const std::vector<unsigned int> &estado, const std::vector<std::vector<unsigned int>> &matriz_distancia);
int calcula_gaschnig(const std::vector<unsigned int> &estado);
int calcula_conflito_linear(const std::vector<unsigned int> &estado, int tamanho_grid);

unsigned long long geraID(const int contagem[5][5], int tamanho_grid);
void gera_matrix_WD(int tamanho_grid, std::unordered_map<unsigned long long, int> &gabarito);
unsigned long long extraiLinhas(std::vector<unsigned int> &estado, int tamanho_grid);
unsigned long long extraiColunas(std::vector<unsigned int> &estado, int tamanho_grid);
int walkingDistance(std::vector<unsigned int> &estado, std::unordered_map<unsigned long long, int> &gabarito, int tamanho_grid);
#endif