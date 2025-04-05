#include "Restricao.h"
#include <iostream>

using namespace std;

Restricao::Restricao(int peso, bool ehForte) {
    this->peso = peso;
    this->ehForte = ehForte;
}

int Restricao::getPeso() {
    return this->peso;
}

bool Restricao::getEhForte() {
    return this->ehForte;
}

void Restricao::imprimeRestricao() {
    cout << this->peso;
}