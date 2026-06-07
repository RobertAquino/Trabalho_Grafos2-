#include "../Bibliotecas/Heuristicas.hpp"
#include <cstddef>

// distancia de Manhattan
int calcula_heuristica(const std::vector<unsigned int> &estado, const std::vector<std::vector<unsigned int>> &matriz_distancia)
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
int calcula_gaschnig(const std::vector<unsigned int> &estado)
{
    // Copia temporaria
    // Obs. Vector ficou muito lento tive que alocar em um vetor estático para ganhar velocidade
    int size = estado.size();
    unsigned int temp[size];

    // Copiamos os valores manualmente
    for (int i = 0; i < size; i++)
    {
        temp[i] = estado[i];
    }

    int swaps = 0;

    while (true)
    {
        // Verifica se o tabuleiro esta ordenado
        bool ordenado = true;
        int primeira_peca_errada = -1;

        for (int i = 0; i < size; i++)
        {
            if (temp[i] != (unsigned int)i)
            {
                ordenado = false;
                if (primeira_peca_errada == -1)
                    primeira_peca_errada = i;
            }
        }

        if (ordenado)
            break;

        // Encontra a posicao do 0 atual
        int pos_zero = -1;
        for (int i = 0; i < size; i++)
        {
            if (temp[i] == 0)
            {
                pos_zero = i;
                break;
            }
        }
        // Aplica as regras de Gaschnig
        if (pos_zero == 0)
        {
            // Regra: 0 na posicao correta, mas jogo inacabado
            // A peca 0 deve ser trocada com a primeira peca que estiver fora do lugar
            std::swap(temp[0], temp[primeira_peca_errada]);
            swaps++;
        }
        else
        {
            // Regra: 0 na posicao errada
            // A peca que deveria estar na posicao atual do 0 deve ser trocada com ele
            int pos_peca_alvo = -1;
            for (int i = 0; i < size; i++)
            {
                if (temp[i] == (unsigned int)pos_zero)
                {
                    pos_peca_alvo = i;
                    break;
                }
            }
            std::swap(temp[pos_zero], temp[pos_peca_alvo]);
            swaps++;
        }
    }

    return swaps;
}

// int calcula_conflito_linear(const std::vector<unsigned int> &estado, int tamanho_grid)
// {
//     int conflitos = 0;

//     // Verifica conflito na linha
//     for (int linha = 0; linha < tamanho_grid; linha++)
//     {
//         // Compara par de peças da linha
//         for (int esq = 0; esq < tamanho_grid - 1; esq++)
//         {
//             for (int dir = esq + 1; dir < tamanho_grid; dir++)
//             {
//                 unsigned int peca_esq = estado[linha * tamanho_grid + esq];
//                 unsigned int peca_dir = estado[linha * tamanho_grid + dir];

//                 // Ignora espaco vazio
//                 if (peca_esq == 0 || peca_dir == 0)
//                     continue;

//                 // Ambas as pecas pertencem a linha
//                 if ((peca_esq / tamanho_grid == (unsigned int)linha) && (peca_dir / tamanho_grid == (unsigned int)linha))
//                 {
//                     // Estão trocadas
//                     if (peca_esq > peca_dir)
//                     {
//                         conflitos += 2;
//                     }
//                 }
//             }
//         }
//     }

//     // Verifica conflito na coluna
//     for (int coluna = 0; coluna < tamanho_grid; coluna++)
//     {
//         for (int cima = 0; cima < tamanho_grid - 1; cima++)
//         {
//             for (int baixo = cima + 1; baixo < tamanho_grid; baixo++)
//             {
//                 unsigned int peca_cima = estado[cima * tamanho_grid + coluna];
//                 unsigned int peca_baixo = estado[baixo * tamanho_grid + coluna];

//                 // Ignora espaco vazio
//                 if (peca_cima == 0 || peca_baixo == 0)
//                     continue;

//                 // Ambas as pecas pertencem a coluna
//                 if ((peca_cima % tamanho_grid == (unsigned int)coluna) && (peca_baixo % tamanho_grid == (unsigned int)coluna))
//                 {
//                      if(peca_cima > peca_baixo)
//                      {
//                          conflitos += 2;
//                      }
//                 }
//             }
//         }
//     }
//     return conflitos;
// }
