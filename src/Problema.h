#pragma once

class Disciplina;
class Curriculo;

#include <string>
#include <vector>
#include "AulaFixa.h"
#include "Curriculo.h"
#include "Disciplina.h"
#include "Indisponibilidade.h"
#include "Restricao.h"
#include "Sala.h"
#include "Turno.h"
#include "Professor.h"

class Problema {
    private:
        std::vector<AulaFixa*> aulasFixas;
        std::vector<Curriculo*> curriculos;
        std::vector<Disciplina*> disciplinas;
        std::vector<Indisponibilidade*> indisponibilidades;
        std::vector<Restricao*> restricoes;
        std::vector<Sala*> salas;
        std::vector<Turno*> turnos;
        std::vector<Professor*> professores;
        int qtdDisciplinas, qtdSalas, qtdLabs, qtdDias, periodosDia, qtdTurnos, qtdCurriculos, qtdAulasFixas, qtdIndisponibilidades, qtdPesos;
        std::string nome;
    public:
        Problema(std::string nomeArquivo);
        ~Problema();
        
        void imprimeProblema();
        int getQtdDisciplinas();
        int getQtdSalas();
        int getQtdLabs();
        int getQtdDias();
        int getPeriodosDia();
        int getQtdTurnos();
        int getQtdCurriculos();
        int getQtdAulasFixas();
        int getQtdIndisponibilidades();
        int getQtdPesos();
        std::vector<AulaFixa*> getAulasFixas();
        std::vector<Curriculo*> getCurriculos();
        std::vector<Disciplina*> getDisciplinas();
        std::vector<Indisponibilidade*> getIndisponibilidades();
        std::vector<Restricao*> getRestricoes();
        std::vector<Sala*> getSalas();
        std::vector<Turno*> getTurnos();
        std::vector<Professor*> getProfessores();
        std::string getNome();
};