#pragma once

#include "TimeSlot.h"
#include "Sala.h"

class TSFO {
    private:
        TimeSlot ts;
        Sala *sala;
        int valor;
    public:
        TSFO(TimeSlot ts, Sala *sala);
        
        // Getters:
        TimeSlot getTimeSlot();
        Sala *getSala();
        int getValor();
        void setValor(int valor);
        
};