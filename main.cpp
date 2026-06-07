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
    MotorBusca motor;

    cout << "8-PUZZLE" << endl;
    vector<Instancia> instancias_8puzzle = parser.inicializaParser("Instances/8puzzle_instances.txt", 3);
    vector<Instancia> instancias_15puzzle = parser.inicializaParser("Instances/15puzzle_instances.txt", 4);

    for (size_t i = 34; i < instancias_15puzzle.size(); i++)
    {
        if (!instancias_15puzzle.empty())
        {
            auto inicio = std::chrono::high_resolution_clock::now();

            cout << "Instancia :" << i << endl;
            motor.executaIDA_estrela(instancias_15puzzle[i], 4);

            auto fim = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
            std::cout << "Tempo: " << duracao.count() << " ms" << std::endl;
        }
        else
        {
            cout << "Falha!" << endl;
            std::cout << "oi";
        } // oi
    }
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