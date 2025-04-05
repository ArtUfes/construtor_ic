#pragma once

#include <string>
#include "TimeSlot.h"

class Indisponibilidade {
    private:
        std::string nomeDisciplina;
        TimeSlot* timeSlot; // Armazena dia e horario em que a aula nao pode ser alocada
    public:
        Indisponibilidade(std::string nomeD, int dia, int horario);
        ~Indisponibilidade();
        std::string getNomeDisciplina();
        TimeSlot* getTimeSlot();
        void imprimeIndisponibilidade();
};
