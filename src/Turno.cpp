#include "Turno.h"
#include <iostream>

using namespace std;

Turno::Turno(string nomeT, int horarioI, int horarioF) {
    this->nomeTurno = nomeT;
    this->horarioInicio = horarioI;
    this->horarioFim = horarioF;
}

string Turno::getNomeTurno() {
    return this->nomeTurno;
}

int Turno::getHorarioInicio() {
    return this->horarioInicio;
}

int Turno::getHorarioFim() {
    return this->horarioFim;
}

void Turno::imprimeTurno() {
    cout << this->nomeTurno << " " << this->horarioInicio << " " << this->horarioFim << endl;
}