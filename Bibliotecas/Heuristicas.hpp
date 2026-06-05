#ifndef HEURISTICAS_HPP
#define HEURISTICAS_HPP

#include <vector>

int calcula_heuristica(const std::vector<unsigned int> &estado, const std::vector<std::vector<unsigned int>> &matriz_distancia);
int calcula_gaschnig(const std::vector<unsigned int> &estado);

#endif