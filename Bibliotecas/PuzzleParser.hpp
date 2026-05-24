#ifndef PUZZLE_PARSER_HPP
#define PUZZLE_PARSER_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "Estruturas.hpp"

using namespace std;

class PuzzleParser
{
public:
    vector<Instancia> inicializaParser(string caminho, int tamanho_grid)
    {
        vector<Instancia> lista_instancias;
        ifstream arquivo(caminho);

        if (!arquivo.is_open())
        {
            cout << "Erro na abertura do arquivo: " << caminho << endl;
            return lista_instancias;
        }

        string linha;
        unsigned num_esperado = tamanho_grid * tamanho_grid;

        while (getline(arquivo, linha))
        {
            if (linha.empty())
                continue;

            stringstream separator(linha);
            vector<unsigned> tabuleiro_temporario;
            unsigned numero;

            while (separator >> numero)
            {
                tabuleiro_temporario.push_back(numero);
            }

            // Validação de segurança: só salva se a linha tiver exatamente 9 ou 16 peças
            if (tabuleiro_temporario.size() == num_esperado)
            {
                Instancia nova_instancia;
                nova_instancia.tabuleiro = tabuleiro_temporario;
                lista_instancias.push_back(nova_instancia);
            }
            else
            {
                cout << "Aviso: Linha ignorada. Encontradas " << tabuleiro_temporario.size()
                     << " pecas em vez de " << num_esperado << "." << endl;
            }
        }

        arquivo.close();
        return lista_instancias;
    }
};
#endif