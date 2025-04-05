#include "Professor.h"
#include "Constantes.h"

#include <iostream>

using namespace std;

Professor::Professor(string nomeP, int idx, int qtdDias, int qtdHorarios) {
    this->nomeProfessor = nomeP;
    this->idx = idx;
    this->qtdDias = qtdDias;
    this->qtdHorarios = qtdHorarios;

    // aloca a qtd de linhas da matriz
    matrizDiaHorarioDisponiveis = new int*[qtdDias];

    // aloca a qtd de colunas da matriz
    for(int i = 0; i < qtdDias; i++) {
        matrizDiaHorarioDisponiveis[i] = new int[qtdHorarios];
    }

    // inicializa a matriz com 1
    for(int i = 0; i < qtdDias; i++) {
        for(int j = 0; j < qtdHorarios; j++) {
            matrizDiaHorarioDisponiveis[i][j] = DISPONIVEL;
        }
    }
}

Professor::~Professor() {
    for(int i = 0; i < this->qtdDias; i++) {
        delete[] this->matrizDiaHorarioDisponiveis[i];
    }
    delete[] this->matrizDiaHorarioDisponiveis;
}

string Professor::getNomeProfessor() {
    return this->nomeProfessor;
}

int Professor::getIdx() {
    return this->idx;
}

int** Professor::getMatrizDiaHorarioDisponiveis() {
    return matrizDiaHorarioDisponiveis;
}

int Professor::getMatrizDiaHorarioDisponiveis(int dia, int horario) {
    return matrizDiaHorarioDisponiveis[dia][horario];
}

void Professor::alocarAulaMatrizDiaHorarioDisponiveis(int dia, int horario) {
    matrizDiaHorarioDisponiveis[dia][horario] = INDISPONIVEL;
}

void Professor::imprimeProfessor() {
    cout << "Nome: " << nomeProfessor << ", Idx: " << idx << endl;
}

void Professor::imprimirMatrizDiaHorarioDisponiveis() {
    cout << "Matriz de disponibilidade do professor " << nomeProfessor << ":" << endl;
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 12; j++) {
            cout << matrizDiaHorarioDisponiveis[i][j] << " ";
        }
        cout << endl;
    }
}