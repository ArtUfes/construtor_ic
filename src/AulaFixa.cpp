#include "AulaFixa.h"
#include <iostream>

using namespace std;

AulaFixa::AulaFixa(string nomeD, int dia, int horario) {
    this->nomeDisciplina = nomeD;
    this->timeSlot = new TimeSlot(dia, horario);
}

string AulaFixa::getNomeDisciplina() {
    return this->nomeDisciplina;
}

TimeSlot* AulaFixa::getTimeSlot() {
    return this->timeSlot;
}

void AulaFixa::imprimeAulaFixa() {
    cout << this->nomeDisciplina << " " << this->timeSlot->getDia() << " " << this->timeSlot->getHorario() << endl;
}

AulaFixa::~AulaFixa() {
    // delete this->timeSlot;
}