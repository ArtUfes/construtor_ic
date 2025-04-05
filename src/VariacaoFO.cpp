#include "VariacaoFO.h"
#include "Constantes.h"
#include "TSFO.h"
#include "Disciplina.h"
#include "Restricao.h"
#include "Curriculo.h"
#include "Turno.h"
#include "Alocacao.h"
#include "Professor.h"
#include <iostream>

using namespace std;

static bool professorTemAulaMatutino(Professor *p, int dia, vector<Turno*> turnos);
static bool professorTemAulaVespertino(Professor *p, int dia, vector<Turno*> turnos);
static bool professorTemAulaNoturno(Professor *p, int dia, vector<Turno*> turnos);

VariacaoFO::VariacaoFO() {
    valor = 0;
}

int VariacaoFO::getValor() {
    return valor;
}

int VariacaoFO::calculaVariacaoTurnoOptativa(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, bool debug) {
    int variacao = 0;
    // Pega o peso da restrição em questão:
    int peso = restricoes[QTD_OBRIGATORIAS]->getPeso();

    if(!d->getEhOptativaEmAlgumCurriculo()) {
        // cout << "Disciplina não é optativa para nenhum currículo! Dentro de calculaVariacaoTurnoOptativa()\n";
        return 0;
    }

    if(tsfo->getTimeSlot().getHorario() < d->getTurnoOptativo()->getHorarioInicio() || tsfo->getTimeSlot().getHorario() > d->getTurnoOptativo()->getHorarioFim()) {
        variacao += peso;
        if (debug) {
            cout << "INFRINGIU A RESTRIÇÃO DE TURNO OPTATIVO NO TURNO DO CURSO!\n";
            cout << "---- INFO ----\n";
            cout << "Disciplina: " << d->getNomeDisciplina() << "\n";
            cout << "Turno da disciplina: " << d->getTurnoOptativo()->getNomeTurno() << "\n";
            cout << "Horário do TSFO: " << tsfo->getTimeSlot().getHorario() << "\n";
            cout << "--------------\n";
        }
    }

    // imprime o dia e horario do tsfo, a disciplina em questão e tambem o valor da variacao:
    // cout << "\n\n Dentro de calculaVariacaoTurnoOptativa()\n";
    // cout << "\n\nDisciplina: " << d->getNomeDisciplina() << ", ";
    // cout << "Dia: " << tsfo->getTimeSlot().getDia() << ", Horario: " << tsfo->getTimeSlot().getHorario() << ", Variacao: " << variacao << "\n\n";

    return variacao;
}

int VariacaoFO::calculaVariacaoAulaMesmaSala(Disciplina *d, std::vector<Alocacao*> aulasAlocadas, TSFO *tsfo, std::vector<Restricao*> restricoes, bool debug) {
    int variacao = 0;
    // Pega o peso da restrição em questão:
    int peso = restricoes[QTD_OBRIGATORIAS + 1]->getPeso();

    // Verifica se a disciplina já foi alocada em alguma sala:
    bool jaFoiAlocada = false;
    bool foiAlocadaMesmaSala = false;
    for(Alocacao *a : aulasAlocadas) {
        if(a->getDisciplina()->getIdx() == d->getIdx()) {
            jaFoiAlocada = true; // Marcamos se a disciplina já foi alocada alguma vez
            if(a->getSala()->getNomeSala() == tsfo->getSala()->getNomeSala()) {
                foiAlocadaMesmaSala = true; // Marcamos se a disciplina já foi alocada na mesma sala do tsfo
                break;
            }
        }
    }

    if(jaFoiAlocada && !foiAlocadaMesmaSala) {
        variacao += peso;
        if (debug) {
            cout << "\tINFRINGIU A RESTRIÇÃO DE AULAS DA DISCIPLINA NA MESMA SALA!\n";
            cout << "\t---- INFO ----\n";
            cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
            cout << "\tSala da disciplina: " << tsfo->getSala()->getNomeSala() << "\n";
            cout << "\t--------------\n";
        }
    }

    // imprime a sala do tsfo, a disciplina em questão e tambem o valor da variacao:
    // cout << "\n\n Dentro de calculaVariacaoAulaMesmaSala()\n";
    // cout << "Disciplina: " << d->getNomeDisciplina() << ", ";
    // cout << "Sala: " << tsfo->getSala()->getNomeSala() << ", Variacao: " << variacao << "";
    
    return variacao;
}

int VariacaoFO::calculaVariacaoAulaMesmoHorario(Disciplina *d, std::vector<Alocacao*> aulasAlocadas, TSFO *tsfo, std::vector<Restricao*> restricoes, bool debug) {
    int variacao = 0;
    // Pega o peso da restrição em questão:
    int peso = restricoes[QTD_OBRIGATORIAS + 2]->getPeso();

   // Verifica se a disciplina já foi alocada em alguma sala:
    bool jaFoiAlocada = false;
    bool foiAlocadaMesmoHorario = false;
    for(Alocacao *a : aulasAlocadas) {
        if(a->getDisciplina()->getIdx() == d->getIdx()) {
            jaFoiAlocada = true; // Marcamos se a disciplina já foi alocada alguma vez
            if(a->getTimeSlot()->getHorario() == tsfo->getTimeSlot().getHorario()) {
                foiAlocadaMesmoHorario = true; // Marcamos se a disciplina já foi alocada na mesma sala do tsfo
                break;
            }
        }
    }

    if(jaFoiAlocada && !foiAlocadaMesmoHorario) {
        variacao += peso;
        if (debug) {
            cout << "\tINFRINGIU A RESTRIÇÃO DE AULAS DA DISCIPLINA NO MESMO HORÁRIO!\n";
            cout << "\t---- INFO ----\n";
            cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
            cout << "\tHorário da disciplina: " << tsfo->getTimeSlot().getHorario() << "\n";
            cout << "\t--------------\n";
        }
    }

    // imprime o dia e horario do tsfo, a disciplina em questão e tambem o valor da variacao:
    // cout << "\n\n Dentro de calculaVariacaoAulaMesmoHorario()\n";
    // cout << "Disciplina: " << d->getNomeDisciplina() << ", " << "Sala: " << tsfo->getSala()->getNomeSala() << ", ";
    // cout << "Dia: " << tsfo->getTimeSlot().getDia() << ", Horario: " << tsfo->getTimeSlot().getHorario() << ", Variacao: " << variacao << "\n\n";

    
    return variacao;
}

int VariacaoFO::calculaVariacaoAulaCurriculoAdjacente(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, bool debug) {
    int variacao = 0;
    // Pega o peso da restrição em questão:
    int peso = restricoes[QTD_OBRIGATORIAS + 3]->getPeso();

    // Pega a matriz de dias e horários disponíveis do currículo da disciplina:
    int **matriz = d->getCurriculo()->getMatrizDiaHorarioDisponiveis();

    // Pega dia e horário do timeSlot da FO:
    int dia = tsfo->getTimeSlot().getDia();
    int horario = tsfo->getTimeSlot().getHorario();

    /* Queremos que as aulas de um mesmo currículo tenham horários adjacentes, sendo assim,
       se no horário anterior e posterior ao horário da aula da disciplina em questão, a
       aula estiver disponível, significa que alocar uma aula no timeSlot em questão irá
       gerar aulas que não são adjacentes. Por isso, adicionamos o peso da restrição na
       variação nesses casos.
    */

    // If para evitar acessar posições inválidas da matriz:
    if(horario > 0 && horario < d->getQtdHorarios() - 1) {
        if(matriz[dia][horario-1] == DISPONIVEL && matriz[dia][horario+1] == DISPONIVEL) {
            variacao += peso;
            if (debug) {
                cout << "\tINFRINGIU A RESTRIÇÃO DE AULAS DO CURRÍCULO ADJACENTES!\n";
                cout << "\t---- INFO ----\n";
                cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
                cout << "\tDia: " << tsfo->getTimeSlot().getDia() << "\n";
                cout << "\tHorário: " << tsfo->getTimeSlot().getHorario() << "\n";
                cout << "\t--------------\n";
            }
        }
    }
    else if(horario == 0) {
        if(matriz[dia][horario+1] == DISPONIVEL) {
            variacao += peso;
            if (debug) {
                cout << "\tINFRINGIU A RESTRIÇÃO DE AULAS DO CURRÍCULO ADJACENTES!\n";
                cout << "\t---- INFO ----\n";
                cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
                cout << "\tDia: " << tsfo->getTimeSlot().getDia() << "\n";
                cout << "\tHorário: " << tsfo->getTimeSlot().getHorario() << "\n";
                cout << "\t--------------\n";
            }
        }
    }
    else if(horario == d->getQtdHorarios() - 1) {
        if(matriz[dia][horario-1] == DISPONIVEL) {
            variacao += peso;
            if (debug) {
                cout << "\tINFRINGIU A RESTRIÇÃO DE AULAS DO CURRÍCULO ADJACENTES!\n";
                cout << "\t---- INFO ----\n";
                cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
                cout << "\tDia: " << tsfo->getTimeSlot().getDia() << "\n";
                cout << "\tHorário: " << tsfo->getTimeSlot().getHorario() << "\n";
                cout << "\t--------------\n";
            }
        }
    }

    // imprime o dia e horario do tsfo, a disciplina em questão e tambem o valor da variacao:
    // cout << "\n\n Dentro de calculaVariacaoAulaCurriculoAdjacente()\n";
    // cout << "Disciplina: " << d->getNomeDisciplina() << ", ";
    // cout << "Dia: " << tsfo->getTimeSlot().getDia() << ", Horario: " << tsfo->getTimeSlot().getHorario() << ", Variacao: " << variacao << "\n\n";

    
    return variacao;
}

int VariacaoFO::calculaVariacaoAulaProfessorAdjacente(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, std::vector<Professor*> professores, bool debug) {
    int variacao = 0;
    // Pega o peso da restrição em questão:
    int peso = restricoes[QTD_OBRIGATORIAS + 4]->getPeso();
    vector<int> profsDisciplina = d->getProfessores();

    // Pega dia e horário do timeSlot da FO:
    int dia = tsfo->getTimeSlot().getDia();
    int horario = tsfo->getTimeSlot().getHorario();

    // Para cada professor da disciplina, verificamos se a aula anterior e posterior estão disponíveis:
    for(int i = 0; i < (int) profsDisciplina.size(); i++) {
        int idx = profsDisciplina[i];
        int **matriz = professores[idx]->getMatrizDiaHorarioDisponiveis();

        // If para evitar acessar posições inválidas da matriz:
        if(horario > 0 && horario < d->getQtdHorarios() - 1) {
            if(matriz[dia][horario-1] == DISPONIVEL && matriz[dia][horario+1] == DISPONIVEL) {
                variacao += peso;
                if (debug) {
                    cout << "\tINFRINGIU A RESTRIÇÃO DE AULAS DO PROFESSOR ADJACENTES!\n";
                    cout << "\t---- INFO ----\n";
                    cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
                    cout << "\tProfessor: " << professores[idx]->getNomeProfessor() << "\n";
                    cout << "\tDia: " << tsfo->getTimeSlot().getDia() << "\n";
                    cout << "\tHorário: " << tsfo->getTimeSlot().getHorario() << "\n";
                    cout << "\t--------------\n";
                }
            }
        }
        else if(horario == 0) {
            if(matriz[dia][horario+1] == DISPONIVEL) {
                variacao += peso;
                if (debug) {
                    cout << "\tINFRINGIU A RESTRIÇÃO DE AULAS DO PROFESSOR ADJACENTES!\n";
                    cout << "\t---- INFO ----\n";
                    cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
                    cout << "\tProfessor: " << professores[idx]->getNomeProfessor() << "\n";
                    cout << "\tDia: " << tsfo->getTimeSlot().getDia() << "\n";
                    cout << "\tHorário: " << tsfo->getTimeSlot().getHorario() << "\n";
                    cout << "\t--------------\n";
                }
            }
        }
        else if(horario == d->getQtdHorarios() - 1) {
            if(matriz[dia][horario-1] == DISPONIVEL) {
                variacao += peso;
                if (debug) {
                    cout << "\tINFRINGIU A RESTRIÇÃO DE AULAS DO PROFESSOR ADJACENTES!\n";
                    cout << "\t---- INFO ----\n";
                    cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
                    cout << "\tProfessor: " << professores[idx]->getNomeProfessor() << "\n";
                    cout << "\tDia: " << tsfo->getTimeSlot().getDia() << "\n";
                    cout << "\tHorário: " << tsfo->getTimeSlot().getHorario() << "\n";
                    cout << "\t--------------\n";
                }
            }
        }
    }

    // imprime o dia e horario do tsfo, a disciplina em questão e tambem o valor da variacao:
    // cout << "\n\n Dentro de calculaVariacaoAulaProfessorAdjacente()\n";
    // cout << "Disciplina: " << d->getNomeDisciplina() << ", ";
    // cout << "Dia: " << tsfo->getTimeSlot().getDia() << ", Horario: " << tsfo->getTimeSlot().getHorario() << ", Variacao: " << variacao << "\n\n";
    // cout << "Professores: ";
    // for(int i = 0; i < (int) profsDisciplina.size(); i++) {
    //     cout << professores[profsDisciplina[i]]->getNomeProfessor() << " ";
    // }
    // cout << "\n\n";

    return variacao;
}

int VariacaoFO::calculaVariacaoSobrecargaProfessor(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, std::vector<Professor*> professores, bool debug) {
    int variacao = 0;
    // Pega o peso da restrição em questão:
    int peso = restricoes[QTD_OBRIGATORIAS + 5]->getPeso();
    vector<int> profsDisciplina = d->getProfessores();

    // Pega o dia do timeSlot da FO:
    int dia = tsfo->getTimeSlot().getDia();

    // Pega os turnos do currículo da disciplina:
    vector<Turno*> turnos = d->getCurriculo()->getTurnos();

    // Se tem menos do que três turnos, obrigatoriamente não teremos aula nos três turnos em um mesmo dia:
    if(turnos.size() < 3) {
        for(int i = 0; i < (int) profsDisciplina.size(); i++) {
            int idx = profsDisciplina[i];
            
            // Se o professor tem aula no turno matutino e noturno, mas não tem aula no vespertino, adicionamos o peso da restrição:
            if(professorTemAulaMatutino(professores[idx], dia, turnos) && !professorTemAulaVespertino(professores[idx], dia, turnos) && professorTemAulaNoturno(professores[idx], dia, turnos)) {
                variacao += peso;
                if (debug) {
                    cout << "\tINFRINGIU A RESTRIÇÃO DE SOBRECARGA DE HORÁRIOS PARA PROFESSORES!\n";
                    cout << "\t---- INFO ----\n";
                    cout << "\tDisciplina: " << d->getNomeDisciplina() << "\n";
                    cout << "\tProfessor: " << professores[idx]->getNomeProfessor() << "\n";
                    cout << "\tDia: " << tsfo->getTimeSlot().getDia() << "\n";
                    cout << "\t--------------\n";
                }
            }
            /*
                IMPLEMENTAR AS OUTRAS VERIFICAÇÕES
            */
        }
    }
    else {
        for(int i = 0; i < (int) profsDisciplina.size(); i++) {
            int idx = profsDisciplina[i];
            /*
                IMPLEMENTAR AS OUTRAS VERIFICAÇÕES
            */

        }
    }

    // imprime o dia e horario do tsfo, a disciplina em questão e tambem o valor da variacao:
    // cout << "\n\n Dentro de calculaVariacaoSobrecargaProfessor()\n";
    // cout << "Disciplina: " << d->getNomeDisciplina() << ", ";
    // cout << "Dia: " << tsfo->getTimeSlot().getDia() << ", Horario: " << tsfo->getTimeSlot().getHorario() << ", Variacao: " << variacao << "\n\n";
    // cout << "Professores: ";
    // for(int i = 0; i < (int) profsDisciplina.size(); i++) {
    //     cout << professores[profsDisciplina[i]]->getNomeProfessor() << " ";
    // }
    // cout << "Turnos: ";
    // for(int i = 0; i < (int) turnos.size(); i++) {
    //     cout << turnos[i]->getNomeTurno() << " ";
    // }
    // cout << "\n\n";
    
    return variacao;
}

int VariacaoFO::calculaVariacaoTotal(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, std::vector<Alocacao*> aulasAlocadas, std::vector<Professor*> professores, bool debug) {
    int variacao = 0;

    variacao += VariacaoFO::calculaVariacaoTurnoOptativa(d, tsfo, restricoes, debug);
    variacao += VariacaoFO::calculaVariacaoAulaMesmaSala(d, aulasAlocadas, tsfo, restricoes, debug);
    variacao += VariacaoFO::calculaVariacaoAulaMesmoHorario(d, aulasAlocadas, tsfo, restricoes, debug);
    variacao += VariacaoFO::calculaVariacaoAulaCurriculoAdjacente(d, tsfo, restricoes, debug);
    variacao += VariacaoFO::calculaVariacaoAulaProfessorAdjacente(d, tsfo, restricoes, professores, debug);
    variacao += VariacaoFO::calculaVariacaoSobrecargaProfessor(d, tsfo, restricoes, professores, debug);
    
    return variacao;
}

static bool professorTemAulaMatutino(Professor *p, int dia, vector<Turno*> turnos) {
    // Procura e pega o turno matutino:
    Turno *turnoMatutino = nullptr;
    for(Turno *t : turnos) {
        if(t->getNomeTurno() == MATUTINO) {
            turnoMatutino = t;
            break;
        }
    }

    if(turnoMatutino == nullptr) {
        // cout << "Não existe turno matutino na instância! Dentro de professorTemAulaMatutino()\n";
        return false;
    }
    
    for(int i = turnoMatutino->getHorarioInicio(); i <= turnoMatutino->getHorarioFim(); i++) {
        if(p->getMatrizDiaHorarioDisponiveis(dia, i) == INDISPONIVEL) {
            return true;
        }
    }
    return false;
}

static bool professorTemAulaVespertino(Professor *p, int dia, vector<Turno*> turnos) {
    // Procura e pega o turno vespertino:
    Turno *turnoVespertino = nullptr;
    for(Turno *t : turnos) {
        if(t->getNomeTurno() == VESPERTINO) {
            turnoVespertino = t;
            break;
        }
    }

    if(turnoVespertino == nullptr) {
        // cout << "Não existe turno vespertino na instância! Dentro de professorTemAulaVespertino()\n";
        return false;
    }
    
    for(int i = turnoVespertino->getHorarioInicio(); i <= turnoVespertino->getHorarioFim(); i++) {
        if(p->getMatrizDiaHorarioDisponiveis(dia, i) == INDISPONIVEL) {
            return true;
        }
    }
    return false;
}

static bool professorTemAulaNoturno(Professor *p, int dia, vector<Turno*> turnos) {
    // Procura e pega o turno noturno:
    Turno *turnoNoturno = nullptr;
    for(Turno *t : turnos) {
        if(t->getNomeTurno() == NOTURNO) {
            turnoNoturno = t;
            break;
        }
    }

    if(turnoNoturno == nullptr) {
        // cout << "Não existe turno noturno na instância! Dentro de professorTemAulaNoturno()\n";
        return false;
    }
    
    for(int i = turnoNoturno->getHorarioInicio(); i <= turnoNoturno->getHorarioFim(); i++) {
        if(p->getMatrizDiaHorarioDisponiveis(dia, i) == INDISPONIVEL) {
            return true;
        }
    }
    return false;
}



