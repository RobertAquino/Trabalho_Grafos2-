#include "../Bibliotecas/Heuristicas.hpp"
#include <cstddef>
#include <math.h>
#include <iostream>

struct WDEntry
{
    unsigned long long id;
    int custo;
};

static WDEntry wd_table[WD_TABLE_SIZE];
static bool wd_inicializado = false;

// distancia de Manhattan
int calcula_heuristica(const std::vector<unsigned int> &estado, const std::vector<std::vector<unsigned int>> &matriz_distancia)
{
    int h = 0;
    int tamanho_grid = std::sqrt(estado.size());

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

int calcula_conflito_linear(const std::vector<unsigned int> &estado, int tamanho_grid)
{
    int conflitos = 0;

    // Verifica conflito na linha
    for (int linha = 0; linha < tamanho_grid; linha++)
    {
        // Compara par de peças da linha
        for (int esq = 0; esq < tamanho_grid - 1; esq++)
        {
            for (int dir = esq + 1; dir < tamanho_grid; dir++)
            {
                unsigned int peca_esq = estado[linha * tamanho_grid + esq];
                unsigned int peca_dir = estado[linha * tamanho_grid + dir];

                // Ignora espaco vazio
                if (peca_esq == 0 || peca_dir == 0)
                    continue;

                // Ambas as pecas pertencem a linha
                if ((peca_esq / tamanho_grid == (unsigned int)linha) && (peca_dir / tamanho_grid == (unsigned int)linha))
                {
                    // Estão trocadas
                    if (peca_esq > peca_dir)
                    {
                        conflitos += 2;
                    }
                }
            }
        }
    }

    // Verifica conflito na coluna
    for (int coluna = 0; coluna < tamanho_grid; coluna++)
    {
        for (int cima = 0; cima < tamanho_grid - 1; cima++)
        {
            for (int baixo = cima + 1; baixo < tamanho_grid; baixo++)
            {
                unsigned int peca_cima = estado[cima * tamanho_grid + coluna];
                unsigned int peca_baixo = estado[baixo * tamanho_grid + coluna];

                // Ignora espaco vazio
                if (peca_cima == 0 || peca_baixo == 0)
                    continue;

                // Ambas as pecas pertencem a coluna
                if ((peca_cima % tamanho_grid == (unsigned int)coluna) && (peca_baixo % tamanho_grid == (unsigned int)coluna))
                {
                    if (peca_cima > peca_baixo)
                    {
                        conflitos += 2;
                    }
                }
            }
        }
    }
    return conflitos;
}
unsigned long long geraID(const int contagem[5][5], int tamanho_grid)
{
    unsigned long long id = 0;

    for (int i = 0; i < tamanho_grid; i++)
    {
        for (int j = 0; j < tamanho_grid; j++)
        {
            id = id << 3;
            id = id | contagem[i][j];
        }
    }
    return id;
}

// Função para SALVAR no vetor
inline void wd_set(unsigned long long id, int valor)
{
    // Mistura as partes altas e baixas do ID com XOR para evitar clusters
    unsigned long long hash = id ^ (id >> 16) ^ (id >> 32);
    unsigned int pos = hash % WD_TABLE_SIZE;

    while (wd_table[pos].id != 0 && wd_table[pos].id != id)
    {
        pos = (pos + 1) % WD_TABLE_SIZE;
    }

    wd_table[pos].id = id;
    wd_table[pos].custo = valor;
}

inline int wd_get(unsigned long long id)
{
    // A mesma mistura na hora de buscar
    unsigned long long hash = id ^ (id >> 16) ^ (id >> 32);
    unsigned int pos = hash % WD_TABLE_SIZE;

    while (wd_table[pos].id != 0)
    {
        if (wd_table[pos].id == id)
            return wd_table[pos].custo;
        pos = (pos + 1) % WD_TABLE_SIZE;
    }
    return 0;
}
void gera_matrix_WD(int tamanho_grid)
{
    if (wd_inicializado)
        return; // TRAVA: Se já rodou, não faz de novo!

    std::memset(wd_table, 0, sizeof(wd_table));

    StateMatrix stateMatrix;
    std::queue<StateMatrix> queue;

    for (int i = 0; i < tamanho_grid; i++)
    {
        for (int j = 0; j < tamanho_grid; j++)
        {
            if (i == j)
            {
                if (i == 0)
                    stateMatrix.matrix[i][j] = tamanho_grid - 1;
                else
                    stateMatrix.matrix[i][j] = tamanho_grid;
            }
            else
                stateMatrix.matrix[i][j] = 0;
        }
    }
    stateMatrix.custo = 0;
    stateMatrix.pos_zero = 0;

    queue.push(stateMatrix);

    unsigned long long id_inicial = geraID(stateMatrix.matrix, tamanho_grid);
    wd_set(id_inicial, 0);

    while (!queue.empty())
    {
        StateMatrix current;
        current = queue.front();
        queue.pop();

        int z_line = current.pos_zero;

        if (z_line > 0)
        {
            for (int i = 0; i < tamanho_grid; i++)
            {
                if (current.matrix[z_line - 1][i] > 0)
                {
                    StateMatrix temp = current;

                    temp.matrix[z_line - 1][i]--;
                    temp.matrix[z_line][i]++;

                    temp.pos_zero = z_line - 1;
                    temp.custo = current.custo + 1;

                    unsigned long long id = geraID(temp.matrix, tamanho_grid);
                    if (id != id_inicial && wd_get(id) == 0)
                    {
                        wd_set(id, temp.custo);
                        queue.push(temp);
                    }
                }
            }
        }

        if (z_line < tamanho_grid - 1)
        {
            for (int i = 0; i < tamanho_grid; i++)
            {
                if (current.matrix[z_line + 1][i] > 0)
                {
                    StateMatrix temp = current;

                    temp.matrix[z_line + 1][i]--;
                    temp.matrix[z_line][i]++;

                    temp.custo = current.custo + 1;
                    temp.pos_zero = z_line + 1;

                    unsigned long long id = geraID(temp.matrix, tamanho_grid);

                    if (id != id_inicial && wd_get(id) == 0)
                    {
                        wd_set(id, temp.custo);
                        queue.push(temp);
                    }
                }
            }
        }
    }
    wd_inicializado = true;
}
int walkingDistance(std::vector<unsigned int> &estado, int tamanho_grid)
{
    if (tamanho_grid == 4)
    {
        unsigned long long id_linhas = 0;
        unsigned long long id_colunas = 0;

        for (int i = 0; i < 16; i++)
        {
            unsigned int peca = estado[i];
            if (peca == 0)
                continue;

            int r_atual = i >> 2;
            int r_alvo = peca >> 2;

            int c_atual = i & 3;
            int c_alvo = peca & 3;

            int shift_linha = 45 - (((r_atual << 2) | r_alvo) * 3);
            int shift_coluna = 45 - (((c_atual << 2) | c_alvo) * 3);

            id_linhas += (1ULL << shift_linha);
            id_colunas += (1ULL << shift_coluna);
        }

        return wd_get(id_linhas) + wd_get(id_colunas);
    }
    else
    {
        int contagem_linhas[5][5] = {0};
        int contagem_colunas[5][5] = {0};
        int total_pecas = tamanho_grid * tamanho_grid;

        for (int i = 0; i < total_pecas; i++)
        {
            unsigned int peca = estado[i];
            if (peca == 0)
                continue;

            contagem_linhas[i / tamanho_grid][peca / tamanho_grid]++;
            contagem_colunas[i % tamanho_grid][peca % tamanho_grid]++;
        }

        unsigned long long id_linhas = geraID(contagem_linhas, tamanho_grid);
        unsigned long long id_colunas = geraID(contagem_colunas, tamanho_grid);

        return wd_get(id_linhas) + wd_get(id_colunas);
    }
}