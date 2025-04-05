#include "TSFO.h"

TSFO::TSFO(TimeSlot ts, Sala *sala) : ts(ts) {
    this->sala = sala;
    this->valor = 0;
}

TimeSlot TSFO::getTimeSlot() {
    return ts;
}

Sala *TSFO::getSala() {
    return sala;
}

int TSFO::getValor() {
    return valor;
}

void TSFO::setValor(int valor) {
    this->valor = valor;
}