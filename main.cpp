#include <iostream>
#include <vector>
#include <iomanip>
#include "Bibliotecas\Estruturas.hpp"
#include "Bibliotecas\PuzzleParser.hpp"
#include "Bibliotecas\MotorBusca.hpp"

using namespace std;

void imprimirTabuleiro(const vector<unsigned> &tabuleiro, int tamanho_grid)
{
    for (int i = 0; i < tabuleiro.size(); i++)
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

    if (!instancias_8puzzle.empty())
    {
        cout << "Instancia [0]" << endl;
        motor.executaA_estrela(instancias_8puzzle[0], 3);
    }
    else
    {
        cout << "Falha!" << endl;
    }

    cout << "15-PUZZLE" << endl;
    vector<Instancia> instancias_15puzzle = parser.inicializaParser("Instances/15puzzle_instances.txt", 4);

    if (!instancias_15puzzle.empty())
    {
        cout << "Instancia [0]" << endl;
        motor.executaA_estrela(instancias_15puzzle[0], 4);
    }
    else
    {
        cout << "Falha!" << endl;
    }

    return 0;
}