#pragma once

#include <string>

class Turno {
    private:
        std::string nomeTurno;
        int horarioInicio, horarioFim;
    public:
        Turno(std::string nomeT, int horarioI, int horarioF);
        std::string getNomeTurno();
        int getHorarioInicio();
        int getHorarioFim();
        void imprimeTurno();
};