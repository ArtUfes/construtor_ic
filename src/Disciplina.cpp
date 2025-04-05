#include <iostream>
#include <string>
#include "Disciplina.h"
#include "Professor.h"
#include "Sala.h"
#include "Constantes.h"

using namespace std;

Disciplina::Disciplina(string nomeD, int qtdP, int qtdAS, int numMDA, int qtdA, int idx, char salaLab, int qtdDias, int qtdHorarios) {
    this->nomeDisciplina = nomeD;
    this->qtdProfessores = qtdP;
    this->qtdAulasSemana = qtdAS;
    this->qtdAulasNaoAlocadas = qtdAS;
    this->numMinimoDiasAula = numMDA;
    this->qtdAlunos = qtdA;
    this->salaOuLab = salaLab;
    this->idx = idx;
    this->qtdTimeSlotsDisponiveis = 0;
    this->qtdDias = qtdDias;
    this->qtdHorarios = qtdHorarios;
    this->turno = nullptr;
    this->curriculo = nullptr;
    this->ehOptativaEmAlgumCurriculo = false;
    this->turnoOptativo = nullptr;

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

Disciplina::~Disciplina() {
    for(int i = 0; i < this->qtdDias; i++) {
        delete[] this->matrizDiaHorarioDisponiveis[i];
    }
    delete[] this->matrizDiaHorarioDisponiveis;
    // if(this->turnoOptativo != nullptr) {
    //     delete this->turnoOptativo;
    // }
}

void Disciplina::setProfessores(vector<int> professores) {
    this->professores = professores;
}

string Disciplina::getNomeDisciplina() {
    return this->nomeDisciplina;
}

int Disciplina::getQtdProfessores() {
    return this->qtdProfessores;
}

int Disciplina::getQtdAulasSemana() {
    return this->qtdAulasSemana;
}

int Disciplina::getNumMinimoDiasAula() {
    return this->numMinimoDiasAula;
}

int Disciplina::getQtdAlunos() {
    return this->qtdAlunos;
}

char Disciplina::getSalaOuLab() {
    return this->salaOuLab;
}

int Disciplina::getIdx() {
    return this->idx;
}

vector<int> Disciplina::getProfessores() {
    return this->professores;
}

vector<int> Disciplina::getAulasFixas() {
    return this->aulasFixas;
}

vector<int> Disciplina::getIndisponibilidades() {
    return this->indisponibilidades;
}

int Disciplina::getQtdTimeSlotsDisponiveis() {
    return this->qtdTimeSlotsDisponiveis;
}

int Disciplina::getMatrizDiaHorarioDisponiveis(int dia, int horario) {
    if (dia >= 0 && dia < qtdDias && horario >= 0 && horario < qtdHorarios) {
        return matrizDiaHorarioDisponiveis[dia][horario];
    }
    else {
        // Imprimir os indices que estão fora do range
        cout << "\n\n\n\n\n\n" << "Indices fora do range: " << dia << " " << horario << "\n\n\n\n\n\n";
        exit(1);
    }
}

int Disciplina::getQtdAulasNaoAlocadas() {
    return this->qtdAulasNaoAlocadas;
}

Curriculo* Disciplina::getCurriculo() {
    return this->curriculo;
}

Turno* Disciplina::getTurno() {
    return this->turno;
}

int Disciplina::getQtdDias() {
    return this->qtdDias;
}

int Disciplina::getQtdHorarios() {
    return this->qtdHorarios;
}

bool Disciplina::getEhOptativaEmAlgumCurriculo() {
    return this->ehOptativaEmAlgumCurriculo;
}

Turno* Disciplina::getTurnoOptativo() {
    return this->turnoOptativo;
}

bool Disciplina::ehSala() {
    return this->salaOuLab == 'S';
}

void Disciplina::setCurriculo(Curriculo *c) {
    this->curriculo = c;
}

void Disciplina::setTurno(Turno *t) {
    this->turno = t;
}

void Disciplina::setEhOptativaEmAlgumCurriculo(bool b) {
    this->ehOptativaEmAlgumCurriculo = b;
}

void Disciplina::setTurnoOptativo(Turno *t) {
    this->turnoOptativo = t;
}

void Disciplina::adicionarAulaFixa(int idx) {
    this->aulasFixas.push_back(idx);
}

void Disciplina::adicionarIndisponibilidade(int idx) {
    this->indisponibilidades.push_back(idx);
}

void Disciplina::alocarAulaMatrizDiaHorarioDisponiveis(int dia, int horario) {
    matrizDiaHorarioDisponiveis[dia][horario] = INDISPONIVEL;
    qtdAulasNaoAlocadas--;
}

void Disciplina::setarIndisponibilidadeMatriz(int dia, int horario) {
    matrizDiaHorarioDisponiveis[dia][horario] = INDISPONIBILIDADE;
}

void Disciplina::atualizaMatrizDiaHorarioDisponiveis(Problema *p) {
    vector<Professor*> professores = p->getProfessores();
    vector<Sala*> salas = p->getSalas();

    // Atualizando em relação aos professores:
    for(int i = 0; i < (int) this->professores.size(); i++) {
        Professor *prof = professores[this->professores[i]];
        for(int j = 0; j < p->getQtdDias(); j++) {
            for(int k = 0; k < p->getPeriodosDia(); k++) {
                if(prof->getMatrizDiaHorarioDisponiveis(j, k) == INDISPONIVEL) {
                    if(matrizDiaHorarioDisponiveis[j][k] == DISPONIVEL) {
                        matrizDiaHorarioDisponiveis[j][k] = INDISPONIVEL;
                    }
                }
            }
        }
    }

    /* Verifica se existe alguma sala que comporta a qtd de alunos da disciplina e que tenha um timeSlot disponível
    para a disciplina. Se não houver, marca o timeSlot como indisponível na matriz de disponibilidade da disciplina
    Ou seja, essa disciplina não possui esse timeSlot disponivel, já que não há nenhuma sala em que a aula possa ser alocada
    nesse timeSlot.
    */
    for(int i = 0; i < p->getQtdDias(); i++) {
        for(int j = 0; j < p->getPeriodosDia(); j++) {
            bool encontrouTimeSlotDisponivel = false;
            for(int k = 0; k < (int) salas.size(); k++) {
                Sala *sala = salas[k];
                if(qtdAlunos <= sala->getCapacidade()) {
                    if(sala->getMatrizDiaHorarioDisponiveis(i, j) == DISPONIVEL) {
                        encontrouTimeSlotDisponivel = true;
                        break;
                    }
                }
            }
            if(!encontrouTimeSlotDisponivel) {
                matrizDiaHorarioDisponiveis[i][j] = INDISPONIVEL;
            }
        }
    }
}

void Disciplina::incrementaQtdTimeSlotsDisponiveis() {
    this->qtdTimeSlotsDisponiveis++;
}

void Disciplina::resetaQtdTimeSlotsDisponiveis() {
    this->qtdTimeSlotsDisponiveis = 0;
}

void Disciplina::imprimeDisciplina(vector<Professor*> p) { // Precisa receber esse vetor para poder imprimir o nome dos profs de fato.
    cout << this->nomeDisciplina << " " << this->qtdProfessores << " ";
    for(int j = 0; j < this->qtdProfessores; j++) {
        cout << p[professores[j]]->getNomeProfessor() << " ";
    }
    cout << this->qtdAulasSemana << " " << this->numMinimoDiasAula << " " << this->qtdAlunos << " " << this->salaOuLab << endl;

    if(this->curriculo != nullptr)
        cout << "Curriculo: " << this->curriculo->getNomeCurriculo() << endl;
    else
        cout << "Curriculo: " << "NULL" << endl;
    if(this->turno != nullptr)
        cout << "Turno: " << this->turno->getNomeTurno() << endl;
    else
        cout << "Turno: " << "NULL" << endl;
    cout << "\n\n";
    
    // Código para imprimir vector de aulas fixas e indisponibilidades de cada disciplina:

    // cout << "Aulas Fixas: " << (int)this->aulasFixas.size() << endl;
    // for(int i = 0; i < (int)this->aulasFixas.size(); i++) {
    //     cout << this->aulasFixas[i];
    //     if(i != (int)this->aulasFixas.size() - 1) {
    //         cout << " ";
    //     }
    // }
    // cout << "\n\n";

    // cout << "\nIndisponibilidades:" << (int)this->indisponibilidades.size() << endl;
    // for(int i = 0; i < (int)this->indisponibilidades.size(); i++) {
    //     cout << this->indisponibilidades[i];
    //     if(i != (int)this->indisponibilidades.size() - 1) {
    //         cout << " ";
    //     }
    // }
    // cout << "\n\n";
}

void Disciplina::imprimirMatrizDiaHorarioDisponiveis() {
    cout << "Matriz de disponibilidade da disciplina " << nomeDisciplina << ":" << endl;
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 12; j++) {
            cout << matrizDiaHorarioDisponiveis[i][j] << " ";
        }
        cout << endl;
    }
}



