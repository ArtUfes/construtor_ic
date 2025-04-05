#include "Curriculo.h"
#include "Constantes.h"
#include <iostream>

using namespace std;

Curriculo::Curriculo(string nomeC, int qtdDObr, int qtdDOpt, int qtdDias, int qtdHorarios, vector<Turno*> turnos) {
    this->nomeCurriculo = nomeC;
    this->qtdDisciplinasObrigatorias = qtdDObr;
    this->qtdDisciplinasOptativas = qtdDOpt;
    this->qtdDias = qtdDias;
    this->qtdHorarios = qtdHorarios;
    this->turnos = turnos;

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

Curriculo::~Curriculo() {
    for(int i = 0; i < this->qtdDias; i++) {
        delete[] this->matrizDiaHorarioDisponiveis[i];
    }
    delete[] this->matrizDiaHorarioDisponiveis;
}

void Curriculo::setDisciplinasObrigatorias(vector<int> disciplinasObrigatorias) {
    this->disciplinasObrigatorias = disciplinasObrigatorias;
}

void Curriculo::setDisciplinasOptativas(vector<int> disciplinasOptativas) {
    this->disciplinasOptativas = disciplinasOptativas;
}

string Curriculo::getNomeCurriculo() {
    return this->nomeCurriculo;
}

string Curriculo::getTurno() {
    return this->turno;
}

vector<Turno*> Curriculo::getTurnos() {
    return this->turnos;
}

int Curriculo::getQtdDisciplinasObrigatorias() {
    return this->qtdDisciplinasObrigatorias;
}

int Curriculo::getQtdDisciplinasOptativas() {
    return this->qtdDisciplinasOptativas;
}

vector<int> Curriculo::getDisciplinasObrigatorias() {
    return this->disciplinasObrigatorias;
}

vector<int> Curriculo::getDisciplinasOptativas() {
    return this->disciplinasOptativas;
}

void Curriculo::imprimeCurriculo(vector<Disciplina*> d) {
    cout << this->nomeCurriculo << " " << this->turno << " " << this->qtdDisciplinasObrigatorias << " ";
    for(int i = 0; i < this->qtdDisciplinasObrigatorias; i++) {
        cout << d[this->disciplinasObrigatorias[i]]->getNomeDisciplina() << " ";
    }
    cout << this->qtdDisciplinasOptativas;
    if(this->qtdDisciplinasOptativas != 0) {
        cout << " ";
    }
    for(int i = 0; i < this->qtdDisciplinasOptativas; i++) {
        cout << d[this->disciplinasOptativas[i]]->getNomeDisciplina();
        if(i != this->qtdDisciplinasOptativas - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

int** Curriculo::getMatrizDiaHorarioDisponiveis() {
    return matrizDiaHorarioDisponiveis;
}

int Curriculo::getMatrizDiaHorarioDisponiveis(int dia, int horario) {
    return matrizDiaHorarioDisponiveis[dia][horario];
}

void Curriculo::alocarAulaMatrizDiaHorarioDisponiveis(int dia, int horario) {
    matrizDiaHorarioDisponiveis[dia][horario] = INDISPONIVEL;
}

void Curriculo::atualizaMatrizDiaHorarioDisponiveis(Problema *p) {
    // Código feito pelo GitHub Copilot (Verificar se é isso que você quer fazer):
    for(int i = 0; i < this->qtdDisciplinasObrigatorias; i++) {
        for(int j = 0; j < p->getDisciplinas()[this->disciplinasObrigatorias[i]]->getQtdDias(); j++) {
            for(int k = 0; k < p->getDisciplinas()[this->disciplinasObrigatorias[i]]->getQtdHorarios(); k++) {
                if(p->getDisciplinas()[this->disciplinasObrigatorias[i]]->getMatrizDiaHorarioDisponiveis(j, k) == INDISPONIVEL) {
                    this->alocarAulaMatrizDiaHorarioDisponiveis(j, k);
                }
            }
        }
    }
    for(int i = 0; i < this->qtdDisciplinasOptativas; i++) {
        for(int j = 0; j < p->getDisciplinas()[this->disciplinasOptativas[i]]->getQtdDias(); j++) {
            for(int k = 0; k < p->getDisciplinas()[this->disciplinasOptativas[i]]->getQtdHorarios(); k++) {
                if(p->getDisciplinas()[this->disciplinasOptativas[i]]->getMatrizDiaHorarioDisponiveis(j, k) == INDISPONIVEL) {
                    this->alocarAulaMatrizDiaHorarioDisponiveis(j, k);
                }
            }
        }
    }
}

void Curriculo::imprimirMatrizDiaHorarioDisponiveis() {
    cout << "Matriz de disponibilidade do curriculo " << nomeCurriculo << ":" << endl;
    for(int i = 0; i < qtdDias; i++) {
        for(int j = 0; j < qtdHorarios; j++) {
            cout << matrizDiaHorarioDisponiveis[i][j] << " ";
        }
        cout << endl;
    }
}