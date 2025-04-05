#pragma once

#include "TimeSlot.h"
#include "Sala.h"
#include "Disciplina.h"

class Alocacao {
    private:
        Disciplina* disciplina;
        Sala* sala;
        TimeSlot* timeSlot;
    public:
        Alocacao(Disciplina *disciplina, Sala *sala, TimeSlot *timeSlot);
        ~Alocacao();

        // Getters:
        Disciplina* getDisciplina();
        Sala* getSala();
        TimeSlot* getTimeSlot();

        void imprimeAlocacao();
};