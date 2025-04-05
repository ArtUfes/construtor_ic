#pragma once

#include "Professor.h"

class TimeSlot {
    private:
        int dia, horario;
    public:
        TimeSlot(int dia, int horario);
        
        // Getters:
        int getDia();
        int getHorario();
        void setDia(int dia);
        void setHorario(int horario);
};