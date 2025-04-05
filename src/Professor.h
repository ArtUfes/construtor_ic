#pragma once

#include <string>

class Professor {
    private:
        std::string nomeProfessor;
        int idx;
        int **matrizDiaHorarioDisponiveis;

        // Variavies auxiliares para a matrizDiaHorarioDisponiveis:
        int qtdDias, qtdHorarios;
    public:
        Professor(std::string nomeP, int idx, int qtdDias, int qtdHorarios);
        ~Professor();

        std::string getNomeProfessor();
        int getIdx();
        int** getMatrizDiaHorarioDisponiveis();
        int getMatrizDiaHorarioDisponiveis(int dia, int horario);
        void alocarAulaMatrizDiaHorarioDisponiveis(int dia, int horario);
        
        // Printers:
        void imprimeProfessor();
        void imprimirMatrizDiaHorarioDisponiveis(); // Acho que não precisará mais



};