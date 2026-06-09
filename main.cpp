#include "Bibliotecas/Estruturas.hpp"
#include "Bibliotecas/PuzzleParser.hpp"
#include "Bibliotecas/MotorBusca.hpp"
#include "Bibliotecas/Heuristicas.hpp" // NÃO SE ESQUEÇA DESTE INCLUDE!
#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>

using namespace std;

// ... (função imprimirTabuleiro mantém-se igual) ...

int main()
{
    PuzzleParser parser;

    vector<Instancia> instancias_8puzzle = parser.inicializaParser("Instances/8puzzle_instances.txt", 3);
    vector<Instancia> instancias_15puzzle = parser.inicializaParser("Instances/15puzzle_instances.txt", 4);

    std::ofstream arquivos[6];
    std::string nomes_h[] = {"", "Manhattan", "Gaschnig", "Conflito_Linear", "Manhattan_E_Conflito_Linear", "Walking_Distance"};
    std::string nomes_arquivos[] = {"", "resultados_Manhattan.csv", "resultados_Gaschnig.csv", "resultados_Conflito.csv", "resultados_Manhattan_Conflito.csv", "resultados_walking_Distance.csv"};

    // 1. GERA A WALKING DISTANCE PRIMEIRO!
    gera_matrix_WD(4);

    // Cria cabeçalho nos arquivos (Abrindo apenas o 5)
    for (int h = 5; h <= 5; h++)
    {
        arquivos[h].open(nomes_arquivos[h], std::ios::app);
        arquivos[h].seekp(0, std::ios::end);
        if (arquivos[h].tellp() == 0)
        {
            arquivos[h] << "Instancia,Algoritmo,Tempo(s),EstadosAvaliados\n";
        }
    }

    // 2. CORRIGE O IF PARA O VETOR CORRETO
    if (!instancias_8puzzle.empty())
    {
        // 3. RODA PELO TAMANHO EXATO DO VETOR (evita Segfault)
        for (size_t i = 0; i < instancias_8puzzle.size(); i++)
        {
            std::string nome_instancia = "Instancia_" + std::to_string(i + 1); // +1 para ficar bonito no log

            for (int aux = 5; aux <= 5; aux++)
            {
                MotorBusca motor;
                std::cout << "\nInstancia " << i << " | Heuristica: " << nomes_h[aux] << std::flush;

                auto inicio = std::chrono::high_resolution_clock::now();

                // Garanta que passa o tamanho 4!
                int estados_avaliados = motor.executaA_estrela(instancias_8puzzle[i], 3, aux);

                auto fim = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duracao = fim - inicio;

                arquivos[aux] << nome_instancia << ",A*," << duracao.count() << "," << estados_avaliados << "\n";
                arquivos[aux].flush();
            }
        }
    }
    else
    {
        cout << "Falha! Ficheiro do 15-puzzle vazio ou nao encontrado." << endl;
    }

    // 4. FECHA O FICHEIRO CORRETO (5)
    for (int h = 5; h <= 5; h++)
    {
        if (arquivos[h].is_open())
        {
            arquivos[h].close();
        }
    }

    return 0;
}