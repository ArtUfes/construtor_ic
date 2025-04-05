#pragma once

#include <string>
#include "TimeSlot.h"

class AulaFixa {
    private:
        std::string nomeDisciplina;
        TimeSlot* timeSlot; // Armazenam dia e horario em que a aula precisa ser alocada
    public:
        AulaFixa(std::string nomeD, int dia, int horario);
        ~AulaFixa();
        std::string getNomeDisciplina();
        TimeSlot* getTimeSlot();
        void imprimeAulaFixa();
};