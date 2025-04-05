#pragma once

class Problema;

#include <string>
#include <vector>
#include "Professor.h"
#include "Problema.h"
#include "Turno.h"
#include "Curriculo.h"

class Disciplina {
    private:
        std::string nomeDisciplina;
        int qtdProfessores, qtdAulasSemana, numMinimoDiasAula, qtdAlunos, idx, qtdAulasNaoAlocadas, qtdTimeSlotsDisponiveis;
        char salaOuLab;
        std::vector<int> professores, aulasFixas, indisponibilidades; // Armazena idx do professor em relação ao vector<Professor> presente na classe Problema.
        int **matrizDiaHorarioDisponiveis; // Matriz que armazena os horários disponíveis para a disciplina. 1 = disponível, 0 = indisponível.

        // Variaveis auxiliares para a matrizDiaHorarioDisponiveis:
        int qtdDias, qtdHorarios;

        // Variavel auxiliar para atrelar o currículo em que a disciplina é obrigatória:
        Curriculo *curriculo;

        // Variavel auxiliar para atrelar o turno do currículo com a disciplina:
        Turno *turno, *turnoOptativo;

        // Variavel auxiliar para saber se a disciplina é optativa em algum currículo:
        bool ehOptativaEmAlgumCurriculo;
    public:
        Disciplina(std::string nomeD, int qtdP, int qtdAS, int numMDA, int qtdA, int idx, char salaLab, int qtdDias, int qtdHorarios);
        ~Disciplina();
        
        void setProfessores(std::vector<int> professores);

        // Getters:
        std::string getNomeDisciplina();
        int getQtdProfessores();
        int getQtdAulasSemana();
        int getNumMinimoDiasAula();
        int getQtdAlunos();
        char getSalaOuLab();
        int getIdx();
        std::vector<int> getProfessores();
        std::vector<int> getAulasFixas();
        std::vector<int> getIndisponibilidades();
        int getMatrizDiaHorarioDisponiveis(int dia, int horario);
        int getQtdAulasNaoAlocadas();
        int getQtdTimeSlotsDisponiveis();
        Curriculo* getCurriculo();
        Turno* getTurno();
        Turno* getTurnoOptativo();
        int getQtdDias();
        int getQtdHorarios();
        bool getEhOptativaEmAlgumCurriculo();

        bool ehSala();

        // Setters:
        void setCurriculo(Curriculo *c);
        void setTurno(Turno *t);
        void setEhOptativaEmAlgumCurriculo(bool b);
        void setTurnoOptativo(Turno *t);
        
        void adicionarAulaFixa(int idx);
        void adicionarIndisponibilidade(int idx);
        void alocarAulaMatrizDiaHorarioDisponiveis(int dia, int horario);
        void setarIndisponibilidadeMatriz(int dia, int horario);
        void atualizaMatrizDiaHorarioDisponiveis(Problema *p);
        void incrementaQtdTimeSlotsDisponiveis();
        void resetaQtdTimeSlotsDisponiveis();


        void imprimeDisciplina(std::vector<Professor*> p);
        void imprimirMatrizDiaHorarioDisponiveis(); // Não teremos mais (Ver como vai imprimir)

};  