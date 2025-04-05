#include "Sala.h"
#include "Constantes.h"
#include <iostream>

using namespace std;

Sala::Sala(string nomeS, int capacidade, int qtdDias, int qtdHorarios, bool ehLaboratorio) {
    this->nomeSala = nomeS;
    this->capacidade = capacidade;
    this->qtdDias = qtdDias;
    this->qtdHorarios = qtdHorarios;
    this->ehLaboratorio = ehLaboratorio;

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

Sala::~Sala() {
    for(int i = 0; i < this->qtdDias; i++) {
        delete[] this->matrizDiaHorarioDisponiveis[i];
    }
    delete[] this->matrizDiaHorarioDisponiveis;
}

string Sala::getNomeSala() {
    return this->nomeSala;
}

int Sala::getCapacidade() const {
    return this->capacidade;
}

int Sala::getMatrizDiaHorarioDisponiveis(int dia, int horario) {
    return matrizDiaHorarioDisponiveis[dia][horario];
}

bool Sala::getEhLaboratorio() const {
    return this->ehLaboratorio;
}

void Sala::imprimeSala(int qtdDias, int qtdHorarios) {
    cout << this->nomeSala << " " << this->capacidade << endl;

    // Ver como vai fazer para imprimir a matriz de disponibilidade
    // Pois precisamos receber de parâmetro a qtd de dias e periodos de dia que está
    // no Problema
    
    cout << "MatrizDiaHorarioDisponiveis: \n";
    for(int i = 0; i < qtdDias; i++) {
        for(int j = 0; j < qtdHorarios; j++) {
            cout << matrizDiaHorarioDisponiveis[i][j] << " ";
        }
        cout << endl;
    }
}

void Sala::alocarAulaMatrizDiaHorarioDisponiveis(int dia, int horario) {
    matrizDiaHorarioDisponiveis[dia][horario] = INDISPONIVEL;
}

bool Sala::verificaDisponibilidade(TimeSlot *ts) {
    if(matrizDiaHorarioDisponiveis[ts->getDia()][ts->getHorario()] == DISPONIVEL) {
        return true;
    }
    return false;
}