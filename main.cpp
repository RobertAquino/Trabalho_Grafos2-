#include <iostream>
#include <vector>
#include <iomanip>
#include "Bibliotecas\Estruturas.hpp"
#include "Bibliotecas\PuzzleParser.hpp"

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

    cout << "8-PUZZLE" << endl;
    vector<Instancia> instancias_8puzzle = parser.inicializaParser("Instances/8puzzle_instances.txt", 3);

    if (!instancias_8puzzle.empty())
    {
        cout << "\nInstancia [0]:" << endl;
        imprimirTabuleiro(instancias_8puzzle[0].tabuleiro, 3);

        cout << "Instancia [1]:" << endl;
        imprimirTabuleiro(instancias_8puzzle[1].tabuleiro, 3);
    }
    else
    {
        cout << "Falha!" << endl;
    }

    cout << "\n15-PUZZLE" << endl;
    vector<Instancia> instancias_15puzzle = parser.inicializaParser("Instances/15puzzle_instances.txt", 4);

    if (!instancias_15puzzle.empty())
    {
        cout << "\nInstancia [0]:" << endl;
        imprimirTabuleiro(instancias_15puzzle[0].tabuleiro, 4);
    }
    else
    {
        cout << "Falha!" << endl;
    }

    return 0;
}