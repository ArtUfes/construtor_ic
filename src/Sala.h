#pragma once

#include <string>
#include "TimeSlot.h"

class Sala {
    private:
        std::string nomeSala;
        int capacidade;
        int **matrizDiaHorarioDisponiveis;
        bool ehLaboratorio;

        // Variavies auxiliares para a matrizDiaHorarioDisponiveis:
        int qtdDias, qtdHorarios;
    public:
        Sala(std::string nomeS, int capacidade, int qtdDias, int qtdHorarios, bool ehLaboratorio);
        ~Sala();

        std::string getNomeSala();
        int getCapacidade() const;
        int getMatrizDiaHorarioDisponiveis(int dia, int horario);
        bool getEhLaboratorio() const;
        void imprimeSala(int qtdDias, int qtdHorarios);
        void alocarAulaMatrizDiaHorarioDisponiveis(int dia, int horario);
        bool verificaDisponibilidade(TimeSlot *ts);
};