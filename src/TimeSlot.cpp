#include "TimeSlot.h"

TimeSlot::TimeSlot(int dia, int horario) {
    this->dia = dia;
    this->horario = horario;
}

int TimeSlot::getDia() {
    return this->dia;
}

int TimeSlot::getHorario() {
    return this->horario;
}

void TimeSlot::setDia(int dia) {
    this->dia = dia;
}

void TimeSlot::setHorario(int horario) {
    this->horario = horario;
}