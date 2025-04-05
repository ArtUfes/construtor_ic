#pragma once

class Disciplina;
class Turno;

#include <string>
#include <vector>
#include "Disciplina.h"


class Curriculo {
    private:
        std::string nomeCurriculo, turno;
        int qtdDisciplinasObrigatorias, qtdDisciplinasOptativas;
        std::vector<int> disciplinasObrigatorias, disciplinasOptativas; // Armazenam idx da disciplina do vector<Disciplina> da classe Problema
        int **matrizDiaHorarioDisponiveis; // Matriz que armazena os horários disponíveis para a disciplina. 1 = disponível, 0 = indisponível.
        std::vector<Turno*> turnos; // Armazena os turnos disponíveis para o curriculo

        // Variaveis auxiliares para a matrizDiaHorarioDisponiveis:
        int qtdDias, qtdHorarios;
    public:
        Curriculo(std::string nomeC, int qtdDObr, int qtdDOpt, int qtdDias, int qtdHorarios, std::vector<Turno*> turnos);
        ~Curriculo();

        void setDisciplinasObrigatorias(std::vector<int> disciplinasObrigatorias);
        void setDisciplinasOptativas(std::vector<int> disciplinasOptativas);
        std::string getNomeCurriculo();
        std::string getTurno();
        std::vector<Turno*> getTurnos();
        int getQtdDisciplinasObrigatorias();
        int getQtdDisciplinasOptativas();
        std::vector<int> getDisciplinasObrigatorias();
        std::vector<int> getDisciplinasOptativas();
        void imprimeCurriculo(std::vector<Disciplina*> d);

        int** getMatrizDiaHorarioDisponiveis();
        int getMatrizDiaHorarioDisponiveis(int dia, int horario);
        void alocarAulaMatrizDiaHorarioDisponiveis(int dia, int horario);

        void atualizaMatrizDiaHorarioDisponiveis(Problema *p);
        void imprimirMatrizDiaHorarioDisponiveis();

        
};