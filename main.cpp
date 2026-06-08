#include "Bibliotecas/Estruturas.hpp"
#include "Bibliotecas/PuzzleParser.hpp"
#include "Bibliotecas/MotorBusca.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>

using namespace std;

void imprimirTabuleiro(const vector<unsigned> &tabuleiro, int tamanho_grid)
{
    for (size_t i = 0; i < tabuleiro.size(); i++)
    {
        if (tabuleiro[i] == 0)
        {
            cout << setw(2) << " " << " ";
        }
        else
        {
            cout << setw(2) << tabuleiro[i] << " ";
        }
        if ((i + 1) % tamanho_grid == 0)
        {
            cout << endl;
        }
    }
}

int main()
{
    PuzzleParser parser;

    vector<Instancia> instancias_8puzzle = parser.inicializaParser("Instances/8puzzle_instances.txt", 3);
    vector<Instancia> instancias_15puzzle = parser.inicializaParser("Instances/15puzzle_instances.txt", 4);

    std::ofstream arquivos[5];
    std::string nomes_h[] = {"", "Manhattan", "Gaschnig", "Conflito_Linear", "Manhattan_E_Conflito_Linear"};
    std::string nomes_arquivos[] = {"", "resultados_Manhattan.csv", "resultados_Gaschnig.csv", "resultados_Conflito.csv", "resultados_Manhattan_Conflito.csv"};

    for (int h = 1; h <= 4; h++)
    {
        arquivos[h].open(nomes_arquivos[h], std::ios::app);
        arquivos[h].seekp(0, std::ios::end);
        if (arquivos[h].tellp() == 0)
        {
            arquivos[h] << "Instancia,Algoritmo,Tempo(s),EstadosAvaliados\n";
        }
    }

    for (int i = 11; i < 12; i++)
    {
        std::string nome_instancia = "Instancia_" + std::to_string(i);
        if (!instancias_15puzzle.empty())
        {
            for (int aux = 1; aux <= 4; aux++)
            {
                MotorBusca motor;
                std::cout << "\nInstancia " << i << " | Heuristica: " << nomes_h[aux] << std::flush;

                auto inicio = std::chrono::high_resolution_clock::now();

                int estados_avaliados = motor.executaIDA_estrela(instancias_15puzzle[i], 4, aux);

                auto fim = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duracao = fim - inicio;

                arquivos[aux] << nome_instancia << ",IDA*," << duracao.count() << "," << estados_avaliados << "\n";
                arquivos[aux].flush();
            }
        }
        else
        {
            cout << "Falha!" << endl;
        }
    }
    for (int h = 1; h <= 4; h++)
        arquivos[h].close();

    // for (size_t i = 34; i < instancias_15puzzle.size(); i++)
    // {
    //     if (!instancias_15puzzle.empty())
    //     {
    //         auto inicio = std::chrono::high_resolution_clock::now();

    //         cout << "Instancia :" << i << endl;
    //         motor.executaIDA_estrela(instancias_15puzzle[i], 4);

    //         auto fim = std::chrono::high_resolution_clock::now();
    //         auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
    //         std::cout << "Tempo: " << duracao.count() << " ms" << std::endl;
    //     }
    //     else
    //     {
    //         cout << "Falha!" << endl;
    //         std::cout << "oi";
    //     } // oi
    // }
    // cout << "15-PUZZLE" << endl;
    // vector<Instancia> instancias_15puzzle = parser.inicializaParser("Instances/15puzzle_instances.txt", 4);

    // if (!instancias_15puzzle.empty())
    // {
    //     cout << "Instancia [0]" << endl;
    //     // motor.executaA_estrela(instancias_15puzzle[0], 4);
    //     motor.executaIDA_estrela(instancias_15puzzle[18], 4);
    // }
    // else
    // {
    //     cout << "Falha!" << endl;
    // }

    return 0;
}