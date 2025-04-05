#include "Alocacao.h"
#include <iostream>

using namespace std;

Alocacao::Alocacao(Disciplina *disciplina, Sala *sala, TimeSlot *timeSlot) {
    this->disciplina = disciplina;
    this->sala = sala;
    this->timeSlot = timeSlot;
}

Alocacao::~Alocacao() {
    delete this->timeSlot;
}

Disciplina* Alocacao::getDisciplina() {
    return this->disciplina;
}

Sala* Alocacao::getSala() {
    return this->sala;
}

TimeSlot* Alocacao::getTimeSlot() {
    return this->timeSlot;
}

void Alocacao::imprimeAlocacao() {
    cout << this->disciplina->getNomeDisciplina() << " ";
    cout << this->timeSlot->getDia() << " ";
    cout << this->timeSlot->getHorario() << " ";
    if(this->sala != nullptr) {
        cout << this->sala->getNomeSala();
    }
    cout << endl;
}
