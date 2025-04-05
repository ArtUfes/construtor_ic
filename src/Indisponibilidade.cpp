#include "Indisponibilidade.h"
#include <iostream>

using namespace std;

Indisponibilidade::Indisponibilidade(string nomeD, int dia, int horario) {
    this->nomeDisciplina = nomeD;
    this->timeSlot = new TimeSlot(dia, horario);
}

string Indisponibilidade::getNomeDisciplina() {
    return this->nomeDisciplina;
}

TimeSlot* Indisponibilidade::getTimeSlot() {
    return this->timeSlot;
}

void Indisponibilidade::imprimeIndisponibilidade() {
    cout << this->nomeDisciplina << " " << this->timeSlot->getDia() << " " << this->timeSlot->getHorario() << endl;
}

Indisponibilidade::~Indisponibilidade() {
    delete this->timeSlot;
}