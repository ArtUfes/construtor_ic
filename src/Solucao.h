#pragma once

#include <string>
#include <vector>
#include "Problema.h"
#include "Alocacao.h"
#include "Disciplina.h"

const int DISPONIVEL = 1;
const int INDISPONIVEL = 0;

class Solucao {
    private:
        int qtdAulasAlocadas, qtdAulasNaoAlocadas;
        int custoSolucao;
        std::vector<int> aulasNaoAlocadas;
        std::vector<Alocacao*> aulasAlocadas;
        std::vector<Alocacao*> AlocacoesDisponiveis;
        int matrizAlocacoesDisponiveis[100][5][12];
        
    public:
        Solucao();
        ~Solucao();
        void geraListaAulasNaoAlocadas(Problema *p);
        void geraListaAlocacoesDisponiveis(Problema *p);
        void geraListaAulasAlocadas(Problema *p);

        void gerarMatrizAlocacoesDisponiveis(Problema *p);

        void imprimeListaAulasNaoAlocadas();
        void imprimeListaAlocacoesDisponiveis(Problema *p);
        void imprimeListaAulasAlocadas();

        // static int gerarNumeroAleatorio(int min, int max);

        // static void sorteiaDiaHorarioDisponivelDisciplina(Disciplina *d, int *dia, int *horario);
        static void imprimirMatrizDiaHorarioDisponiveis(int **matriz, Problema *p);

};