#include "Solucao.h"
#include <iostream>
#include <algorithm>
#include <random>
#include "TSFO.h"
#include "VariacaoFO.h"

using namespace std;

static int gerarNumeroAleatorio(int min, int max);
static void marcaIndisponibilidadesDasDisciplinas(Problema *p);
static void alocaAula(Problema *p, Disciplina *d, TimeSlot *ts, Sala *sala, vector<Alocacao*>& aulasAlocadas);
static bool verificaSeTodosProfessoresEstaoDisponiveisNoTimeSlot(Problema *p, Disciplina *d, TimeSlot *ts);
static void alocaAulasFixas(Problema *p, vector<Alocacao*>& aulasAlocadas);
static void preencheVectorDisciplinasComAulasNaoAlocadas(Problema *p, vector<Disciplina*>& disciplinasComAulasNaoAlocadas);
static void contaQtdTimeSlotsDisponiveis(Problema *p, vector<Disciplina*>& disciplinasComAulasNaoAlocadas);
static Disciplina *retornaDisciplinaComMenosTimeSlotsDisponiveis(vector<Disciplina*>& disciplinasComAulasNaoAlocadas);
static void alocaAulasNaoAlocadas(Problema *p, vector<Alocacao*>& aulasAlocadas);
static TimeSlot sorteiaTimeSlotDisponivelDisciplina(Disciplina *d);
static vector<TSFO> retornaTimeSlotsDisponiveisDisciplina(Problema *p, Disciplina *d);

Solucao::Solucao() {
    qtdAulasAlocadas = 0;
    qtdAulasNaoAlocadas = 0;
    custoSolucao = 0;
}

Solucao::~Solucao() {
    for(int i = 0; i < (int) aulasAlocadas.size(); i++) {
        delete aulasAlocadas[i];
    }
}

void Solucao::geraListaAulasNaoAlocadas(Problema *p) {
    std::vector<Disciplina*> disciplinas = p->getDisciplinas();

    for(int i = 0; i < (int) disciplinas.size(); i++) {
        for(int j = 0; j < disciplinas[i]->getQtdAulasSemana(); j++) {
            aulasNaoAlocadas.push_back(i);
        }
    }
    this->qtdAulasNaoAlocadas = aulasNaoAlocadas.size();
}

void Solucao::geraListaAlocacoesDisponiveis(Problema *p) {
    for(int i = 0; i < p->getQtdDias(); i++) {
        for(int j = 0; j < p->getPeriodosDia(); j++) {
            for(int k = 0; k < (int) p->getSalas().size(); k++) {
                Alocacao *a = new Alocacao(p->getDisciplinas()[0], p->getSalas()[k], new TimeSlot(i, j));
                AlocacoesDisponiveis.push_back(a);
            }
        }
    }

}

// Função auxiliar que marca as indisponibilidades de cada disciplina na matriz de disponibilidade delas:
static void marcaIndisponibilidadesDasDisciplinas(Problema *p) {
    vector<Indisponibilidade*> indisponibilidades = p->getIndisponibilidades();
    vector<Disciplina*> disciplinas = p->getDisciplinas();

    for(int i = 0; i < p->getQtdDisciplinas(); i++) {
        vector<int> indiDisc = disciplinas[i]->getIndisponibilidades();
        for(int j = 0; j < (int) indiDisc.size(); j++) {
            disciplinas[i]->setarIndisponibilidadeMatriz(indisponibilidades[indiDisc[j]]->getTimeSlot()->getDia(), indisponibilidades[indiDisc[j]]->getTimeSlot()->getHorario());
        }
    }
}

// Função auxiliar que aloca uma aula e atualiza as matrizes de disponibilidade:
static void alocaAula(Problema *p, Disciplina *d, TimeSlot *ts, Sala *sala, vector<Alocacao*>& aulasAlocadas) {
    // Pegando alguns vectors que precisaremos:
    vector<Disciplina*> disciplinas = p->getDisciplinas();
    vector<Professor*> professores = p->getProfessores();

    vector<int> profsDisciplina = d->getProfessores();

    for(int l = 0; l < (int) profsDisciplina.size(); l++) { // Percorremos por todos os professores da disciplina
        professores[profsDisciplina[l]]->alocarAulaMatrizDiaHorarioDisponiveis(ts->getDia(), ts->getHorario()); // Indisponibilizamos o timeSlot da aula alocada para o professor
    }
    sala->alocarAulaMatrizDiaHorarioDisponiveis(ts->getDia(), ts->getHorario()); // Indisponibilizamos o timeSlot da aula alocada para a sala
    d->alocarAulaMatrizDiaHorarioDisponiveis(ts->getDia(), ts->getHorario()); // Indisponibilizamos o timeSlot da aula alocada para a disciplina
    d->getCurriculo()->alocarAulaMatrizDiaHorarioDisponiveis(ts->getDia(), ts->getHorario()); // Indisponibilizamos o timeSlot da aula alocada para o curriculo
    for(int l = 0; l < (int) disciplinas.size(); l++) { // Percorremos por todas as disciplinas
        disciplinas[l]->atualizaMatrizDiaHorarioDisponiveis(p); // Atualizamos a matriz de disponibilidade de todas as disciplinas
    }
    Alocacao *a = new Alocacao(d, sala, ts); // Criamos a alocacao
    aulasAlocadas.push_back(a); // Adicionamos a alocacao na lista de aulas alocadas
}

static bool verificaSeTodosProfessoresEstaoDisponiveisNoTimeSlot(Problema *p, Disciplina *d, TimeSlot *ts) {
    vector<Professor*> professores = p->getProfessores(); // Pegamos os professores do problema
    vector<int> profsDisciplina = d->getProfessores(); // Pegamos os professores da disciplina

    for(int l = 0; l < (int) profsDisciplina.size(); l++) { // Percorremos por todos os professores da disciplina
        if(professores[profsDisciplina[l]]->getMatrizDiaHorarioDisponiveis(ts->getDia(), ts->getHorario()) == INDISPONIVEL) { // Verificamos se o professor não está disponível
            return false; // Se algum professor não estiver disponível, retornamos false
        }
    }

    return true; // Se todos professores estiverem disponíveis, retornamos true
}

static void alocaAulasFixas(Problema *p, vector<Alocacao*>& aulasAlocadas) {
    vector<AulaFixa*> aulasFixas = p->getAulasFixas();
    vector<Disciplina*> disciplinas = p->getDisciplinas();
    vector<Sala*> salas = p->getSalas();

    for(int i = 0; i < (int) disciplinas.size(); i++) { // Percorremos por todas as disciplinas existentes:
        Disciplina *d = disciplinas[i]; // Pegamos uma disciplina do vector
        vector<int> aulasFixasDisciplina = d->getAulasFixas(); // Pegamos as aulas fixas dessa disciplina
        for(int j = 0; j < (int) aulasFixasDisciplina.size(); j++) { // Percorremos por todas as aulas fixas dessa disciplina
            int k = d->ehSala() ? 0 : p->getQtdSalas(); // Se a disciplina for uma sala, começamos a percorrer pelas salas, senão, pelos laboratórios
            int parada = d->ehSala() ? p->getQtdSalas() : p->getQtdSalas() + p->getQtdLabs(); // Se a disciplina for uma sala, a parada é a quantidade de salas, senão, a quantidade de laboratórios mais a qtd de salas
            for(; k < parada; k++) { // Para cada aulaFixa, percorremos por todas as salas existentes
                 if(d->getQtdAlunos() <= salas[k]->getCapacidade()) { // Verificamos se a capacidade da sala é suficiente para a disciplina
                    TimeSlot *ts = aulasFixas[aulasFixasDisciplina[j]]->getTimeSlot(); // Pegamos o timeSlot da aula fixa
                    if(salas[k]->getMatrizDiaHorarioDisponiveis(ts->getDia(), ts->getHorario()) == DISPONIVEL) { // Verificamos se não há nenhuma aula alocada nessa sala nesse timeSlot
                        // Precisaremos agora verificar se todos professores da disciplina estão disponíves para o timeSlot
                        if(verificaSeTodosProfessoresEstaoDisponiveisNoTimeSlot(p, d, ts)) { // Se todos professores da disciplina estiverem disponíveis para o timeSlot, alocamos a aula
                            alocaAula(p, d, ts, salas[k], aulasAlocadas);
                        }
                    }
                }
            }
        }
    }
}

static void preencheVectorDisciplinasComAulasNaoAlocadas(Problema *p, vector<Disciplina*>& disciplinasComAulasNaoAlocadas) {
    vector<Disciplina*> disciplinas = p->getDisciplinas();

    for(int i = 0; i < (int) disciplinas.size(); i++) {
        if(disciplinas[i]->getQtdAulasNaoAlocadas() > 0) {
            disciplinas[i]->resetaQtdTimeSlotsDisponiveis();
            disciplinasComAulasNaoAlocadas.push_back(disciplinas[i]);
        }
    }
}

static void contaQtdTimeSlotsDisponiveis(Problema *p, vector<Disciplina*>& disciplinasComAulasNaoAlocadas) {
    vector<Sala*> salas = p->getSalas();

    for(int i = 0; i < (int) disciplinasComAulasNaoAlocadas.size(); i++) {
        for(int j = 0; j < p->getQtdDias(); j++) {
            for(int k = 0; k < p->getPeriodosDia(); k++) {
                int l = disciplinasComAulasNaoAlocadas[i]->ehSala() ? 0 : p->getQtdSalas();
                int parada = disciplinasComAulasNaoAlocadas[i]->ehSala() ? p->getQtdSalas() : p->getQtdSalas() + p->getQtdLabs();
                for(; l < parada; l++) {
                    if(salas[l]->getMatrizDiaHorarioDisponiveis(j, k) == DISPONIVEL) {
                        if(disciplinasComAulasNaoAlocadas[i]->getMatrizDiaHorarioDisponiveis(j, k) == DISPONIVEL) {
                            Curriculo *c = disciplinasComAulasNaoAlocadas[i]->getCurriculo();
                            // Com essa estrutura, estamos ignorando disciplinas que são optativas para o curriculo:
                            if(c == nullptr) {
                                cout << "Curriculo nulo" << endl;
                            }
                            else {
                                if(disciplinasComAulasNaoAlocadas[i]->getCurriculo()->getMatrizDiaHorarioDisponiveis(j, k) == DISPONIVEL) {
                                    disciplinasComAulasNaoAlocadas[i]->incrementaQtdTimeSlotsDisponiveis();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    cout << "\n\n";
}


static Disciplina *retornaDisciplinaComMenosTimeSlotsDisponiveis(vector<Disciplina*>& disciplinasComAulasNaoAlocadas) {
    Disciplina *disciplinaComMenorQtdTimeSlotsDisponiveis = disciplinasComAulasNaoAlocadas[0]; // Começa com a primeira disciplina não alocada
        for(int i = 1; i < (int) disciplinasComAulasNaoAlocadas.size(); i++) { // Avalia a partir da segunda disciplina não alocada
            if(disciplinasComAulasNaoAlocadas[i]->getQtdTimeSlotsDisponiveis() < disciplinaComMenorQtdTimeSlotsDisponiveis->getQtdTimeSlotsDisponiveis()) {
                disciplinaComMenorQtdTimeSlotsDisponiveis = disciplinasComAulasNaoAlocadas[i];
            }
        }
    return disciplinaComMenorQtdTimeSlotsDisponiveis;
}

static void alocaAulasNaoAlocadas(Problema *p, vector<Alocacao*>& aulasAlocadas) {
    vector<Sala*> salas = p->getSalas();
    vector<Professor*> professores = p->getProfessores();

    /*
    Agora precisamos:
        1°- Verificar quais disciplinas ainda possuem aulasNaoAlocadas;
        2°- Fazer a contagem de timeSlots disponíveis para cada uma dessas disciplinas;
        3°- Pegar a disciplina que possuem menos timeSlots disponíveis;
        
        Agora precisamos trocar essa etapa 4 para fazer com que nós classifiquemos cada timeSlot disponível
        da disciplina em relação a variação da FO caso nós alocassemos a aula nesse timeSlot, e então escolher
        o timeSlot que minimiza a variação da FO.

        4°- Para essa disciplina, sorteamos um timeSlot aleatorio, dentre os disponíveis para a disciplina
        e alocamos a aula para essa disciplina;


        5°- Atualizamos as listas de aulas alocadas e não alocadas;
        6°- Agora repetimos o processo até não haver mais disciplinas no vector aulasNaoAlocadas;
    */
    int FO = 0;
    while(true) {
        // 1°- Verificar quais disciplinas ainda possuem aulasNaoAlocadas:
        vector<Disciplina*> disciplinasComAulasNaoAlocadas;
        preencheVectorDisciplinasComAulasNaoAlocadas(p, disciplinasComAulasNaoAlocadas);
        
        // Condição de parada do while:
        if((int) disciplinasComAulasNaoAlocadas.size() == 0) {
            break;
        }

        // Imprime a lista de disciplinas com aulas não alocadas
        // cout << "Disciplinas com aulas não alocadas: ";
        // for(int i = 0; i < (int) disciplinasComAulasNaoAlocadas.size(); i++) {
        //     cout << disciplinasComAulasNaoAlocadas[i]->getNomeDisciplina() << " ";
        // }

        // 2°- Fazer a contagem de timeSlots disponíveis para cada uma dessas disciplinas:
        contaQtdTimeSlotsDisponiveis(p, disciplinasComAulasNaoAlocadas);

        // Imprimir a quantidade de timeSlots disponíveis para cada disciplina
        // cout << "\n\nQuantidade de timeSlots disponíveis para cada disciplina:\n";
        // for(int i = 0; i < (int) disciplinasComAulasNaoAlocadas.size(); i++) {
        //     cout << disciplinasComAulasNaoAlocadas[i]->getNomeDisciplina() << ": " << disciplinasComAulasNaoAlocadas[i]->getQtdTimeSlotsDisponiveis() << endl;
        // }

        // 3°- Pegar a disciplina que possuem menos timeSlots disponíveis:
        Disciplina *disciplinaComMenorQtdTimeSlotsDisponiveis = retornaDisciplinaComMenosTimeSlotsDisponiveis(disciplinasComAulasNaoAlocadas);


        bool quebraWhile = false;
        while(!quebraWhile) {
            // Imprimir a disciplina com menor quantidade de timeSlots disponíveis
            cout << "\nDisciplina com menor quantidade de timeSlots disponíveis: " << disciplinaComMenorQtdTimeSlotsDisponiveis->getNomeDisciplina() << endl;

            // Vamos agora iniciar a mudança da etapa 4. Para isso, iniciamos construindo um vector<TimeSlot>
            // que armazenará todos os timeSlots disponíveis para a disciplina:
            vector<TSFO> timeSlotsDisponiveis = retornaTimeSlotsDisponiveisDisciplina(p, disciplinaComMenorQtdTimeSlotsDisponiveis);

            if(timeSlotsDisponiveis.size() == 0) {
                cout << "Nenhum timeSlot disponível para a disciplina" << endl;
                
                // Imprimir todas as alocações feitas:
                cout << "Aulas alocadas: " << endl;
                for(int i = 0; i < (int) aulasAlocadas.size(); i++) {
                    aulasAlocadas[i]->imprimeAlocacao();
                }



                exit(1);
            }
            else {
                cout << timeSlotsDisponiveis.size() << " timeSlots disponíveis para a disciplina" << endl;
                // exit(2);
            }

            // Calcular e setar o valor da variacao da FO nos timeSlots disponíveis para a disciplina e imprimir:
            // cout << "TimeSlots disponíveis para a disciplina: " + disciplinaComMenorQtdTimeSlotsDisponiveis->getNomeDisciplina() << ":\n";
            for(int i = 0; i < (int) timeSlotsDisponiveis.size(); i++) {
                timeSlotsDisponiveis[i].setValor(VariacaoFO::calculaVariacaoTotal(disciplinaComMenorQtdTimeSlotsDisponiveis, &timeSlotsDisponiveis[i], p->getRestricoes(), aulasAlocadas, p->getProfessores(), false));
                // cout << "Dia: " << timeSlotsDisponiveis[i].getTimeSlot().getDia() << ", Horario: " << timeSlotsDisponiveis[i].getTimeSlot().getHorario() << ", Sala: " << timeSlotsDisponiveis[i].getSala()->getNomeSala() << ", Valor: " << timeSlotsDisponiveis[i].getValor() << "\n";
            }
            
            

            // Achar o timeSlot que minimiza a variacao da FO:
            TSFO *tsfo = &timeSlotsDisponiveis[0];
            for(int i = 1; i < (int) timeSlotsDisponiveis.size(); i++) {
                if(tsfo->getValor() == 0) {
                    break;
                }
                if(timeSlotsDisponiveis[i].getValor() < tsfo->getValor()) {
                    tsfo = &timeSlotsDisponiveis[i];
                }
            }

            // cout << "Valor do TSFO: " << tsfo->getValor() << endl;
            if(tsfo == nullptr) {
                cout << "Nenhum timeSlot minimiza a variacao da FO" << endl;
                exit(1);
            }

            // Imprimir o timeSlot que minimiza a variacao da FO:
            cout << "TimeSlot que minimiza a variacao da FO: Dia: " << tsfo->getTimeSlot().getDia() << ", Horario: " << tsfo->getTimeSlot().getHorario() << ", Sala: " << tsfo->getSala()->getNomeSala() << ", Valor: " << tsfo->getValor() << "\n";
            tsfo->setValor(VariacaoFO::calculaVariacaoTotal(disciplinaComMenorQtdTimeSlotsDisponiveis, tsfo, p->getRestricoes(), aulasAlocadas, p->getProfessores(), true));

            // Agora precisamos alocar a aula no timeSlot que minimiza a variacao da FO:
            alocaAula(p, disciplinaComMenorQtdTimeSlotsDisponiveis, new TimeSlot(tsfo->getTimeSlot().getDia(), tsfo->getTimeSlot().getHorario()), tsfo->getSala(), aulasAlocadas);
            FO += tsfo->getValor();
            quebraWhile = true;


            // // 4°- Para essa disciplina, sorteamos um timeSlot aleatorio, dentre os disponíveis para a disciplina
            // // e alocamos a aula para essa disciplina:
            // TimeSlot ts = sorteiaTimeSlotDisponivelDisciplina(disciplinaComMenorQtdTimeSlotsDisponiveis);

            // int k = disciplinaComMenorQtdTimeSlotsDisponiveis->ehSala() ? 0 : p->getQtdSalas();
            // int parada = disciplinaComMenorQtdTimeSlotsDisponiveis->ehSala() ? p->getQtdSalas() : p->getQtdSalas() + p->getQtdLabs();
            // for(; k < parada; k++) { // Percorremos por todas as salas existentes
            //     // Prints para debbugar:
            //     // cout << "-----------------------\n";
            //     // salas[k]->imprimeSala(p->getQtdDias(), p->getPeriodosDia());
            //     // cout << "\nDiaSorteado: " << ts.getDia() << ", HorarioSorteado: " << ts.getHorario() << endl;

            //     // Comentando para alocar mesmo se não couber na sala: if(disciplinaComMenorQtdTimeSlotsDisponiveis->getQtdAlunos() <= salas[k]->getCapacidade()) { // Verificamos se a capacidade da sala é suficiente para a disciplina
            //         if(salas[k]->verificaDisponibilidade(&ts)) { // Verificamos se não há nenhuma aula alocada nessa sala nesse timeSlot
            //             if(verificaSeTodosProfessoresEstaoDisponiveisNoTimeSlot(p, disciplinaComMenorQtdTimeSlotsDisponiveis, &ts)) { // Se todos professores da disciplina estiverem disponíveis para o timeSlot, alocamos a aula
            //                 // Agora precisamos avaliar se há disponibilidade quanto ao curriculo em que a disciplina é obrigatória:
            //                 if(disciplinaComMenorQtdTimeSlotsDisponiveis->getCurriculo()->getMatrizDiaHorarioDisponiveis(ts.getDia(), ts.getHorario()) == DISPONIVEL) {
            //                     TimeSlot *ts_alocado = new TimeSlot(ts.getDia(), ts.getHorario());
            //                     alocaAula(p, disciplinaComMenorQtdTimeSlotsDisponiveis, ts_alocado, salas[k], aulasAlocadas);
            //                     quebraWhile = true;
            //                     cout << "Aula alocada com sucesso!" << endl;
            //                 }
            //                 // Elses com prints para debuggar:
            //                 else {
            //                     // cout << "Alguma aula de uma disciplina de mesmo Curriculo ja esta alocada nesse timeSlot" << endl;
            //                 }
            //             }
            //             else {
            //                 // cout << "Professores não disponíveis" << endl;
            //             }
            //         }
            //         else {
            //             // cout << "Sala já possui aula alocada nesse timeSlot" << endl;
                    
            //         }
                // } Comentando para alocar mesmo se não couber na sala:
                // else {
                //     cout << "Sala não comporta a quantidade de alunos da disciplina" << endl;
                // }
                // cout << "-----------------------\n\n";
            // }
        }
    }
    cout << "\n\nFO: " << FO << endl;
}


void Solucao::geraListaAulasAlocadas(Problema *p) {
    // Iniciamos marcando as indisponibilidades:
    marcaIndisponibilidadesDasDisciplinas(p);

    // Depois alocamos as aulas fixas:
    alocaAulasFixas(p, this->aulasAlocadas);

    // Por fim, alocamos as aulas que sobraram, até que todas tenham sido alocadas:
    alocaAulasNaoAlocadas(p, this->aulasAlocadas);
}

void Solucao::gerarMatrizAlocacoesDisponiveis(Problema *p) {
    for(int i = 0; i < p->getQtdDias(); i++) {
        for(int j = 0; j < p->getPeriodosDia(); j++) {
            for(int k = 0; k < (int) p->getSalas().size(); k++) {
                matrizAlocacoesDisponiveis[i][j][k] = DISPONIVEL;
            }
        }
    }
}

void Solucao::imprimeListaAulasNaoAlocadas() {
    // cout << endl << "\nqtdAulasNaoAlocadas: " << this->qtdAulasNaoAlocadas << ", custoSolucao: " << this->custoSolucao << endl;

    // cout << "ListaAulasNaoAlocadas: ";
    // for(int i = 0; i < (int) aulasNaoAlocadas.size(); i++) {
    //     cout << aulasNaoAlocadas[i] << " ";
    //     if(i != (int) aulasNaoAlocadas.size() - 1) {
    //         cout << " ";
    //     }
    // }
    // cout << "\n\n";
}

void Solucao::imprimeListaAlocacoesDisponiveis(Problema *p) {
    // cout << endl << "\nAlocacoesDisponiveis: " << this->qtdAlocacoes << ", custoSolucao: " << this->custoSolucao << endl;
}

void Solucao::imprimeListaAulasAlocadas() {
    // cout << endl << "\nAulasAlocadas: " << (int) aulasAlocadas.size() << endl;
    for(int i = 0; i < (int) aulasAlocadas.size(); i++) {
        aulasAlocadas[i]->imprimeAlocacao();
    }
    cout << endl;
}

static int gerarNumeroAleatorio(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);

    return distr(gen);
}

static TimeSlot sorteiaTimeSlotDisponivelDisciplina(Disciplina *d) {
    while(true) {
        int diaSorteado = gerarNumeroAleatorio(0, d->getQtdDias() - 1); // qtdDias - 1 para nao sair do range
        int horarioSorteado;

        // Se temos um turno atrelado a disciplina, sorteamos um horário dentro do turno:
        if(d->getTurno() != nullptr) {
            horarioSorteado = gerarNumeroAleatorio(d->getTurno()->getHorarioInicio(), d->getTurno()->getHorarioFim()); // Não coloca o -1 pois o turno já considera isso
        }
        // Se nao temos um turno atrelado a disciplina, sorteamos um horário dentro da qtd de horários da disciplina (que no final é a qtdHorarios da instancia):
        else {
            horarioSorteado = gerarNumeroAleatorio(0, d->getQtdHorarios() - 1); // qtdHorarios - 1 para nao sair do range
        }

        if(d->getMatrizDiaHorarioDisponiveis(diaSorteado, horarioSorteado) == DISPONIVEL) {
            return TimeSlot(diaSorteado, horarioSorteado);
        }
        cout << "TimeSlot indisponível, sorteando novamente..." << endl;
    }
}

static vector<TSFO> retornaTimeSlotsDisponiveisDisciplina(Problema *p, Disciplina *d) {
    vector<TSFO> timeSlotsDisponiveis;
    vector<Sala*> salas = p->getSalas();

    for(int i = 0; i < d->getQtdDias(); i++) {
        for(int j = d->getTurno()->getHorarioInicio(); j <= d->getTurno()->getHorarioFim(); j++) {
            if(d->getMatrizDiaHorarioDisponiveis(i, j) == DISPONIVEL) {
                TimeSlot ts(i, j); // Se o timeSlot estiver disponível, criamos um objeto TimeSlot para fazer outras verificações
                // Agora precisamos avaliar se há disponibilidade quanto ao curriculo em que a disciplina é obrigatória:
                if(d->getCurriculo()->getMatrizDiaHorarioDisponiveis(ts.getDia(), ts.getHorario()) == DISPONIVEL) {
                    if(verificaSeTodosProfessoresEstaoDisponiveisNoTimeSlot(p, d, &ts)) { // Se todos professores da disciplina estiverem disponíveis para o timeSlot, alocamos a aula
                        int k = d->ehSala() ? 0 : p->getQtdSalas();
                        int parada = d->ehSala() ? p->getQtdSalas() : p->getQtdSalas() + p->getQtdLabs();
                        for(; k < parada; k++) { // Percorremos por todas as salas existentes
                            // Prints para debbugar:
                            // cout << "-----------------------\n";
                            // salas[k]->imprimeSala(p->getQtdDias(), p->getPeriodosDia());
                            // cout << "\nDiaSorteado: " << ts.getDia() << ", HorarioSorteado: " << ts.getHorario() << endl;

                            if(d->getQtdAlunos() <= salas[k]->getCapacidade()) { // Verificamos se a capacidade da sala é suficiente para a disciplina
                                if(salas[k]->verificaDisponibilidade(&ts)) { // Verificamos se não há nenhuma aula alocada nessa sala nesse timeSlot
                                    timeSlotsDisponiveis.push_back(TSFO(ts, salas[k]));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return timeSlotsDisponiveis;
}

void Solucao::imprimirMatrizDiaHorarioDisponiveis(int **matriz, Problema *p) {
    for(int i = 0; i < p->getQtdDias(); i++) {
        for(int j = 0; j < p->getPeriodosDia(); j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}