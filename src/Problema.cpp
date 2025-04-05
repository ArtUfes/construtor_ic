#include "Problema.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Problema::Problema(string nomeArquivo) {
    string lixo;
    ifstream entrada;

    entrada.open(nomeArquivo);

    if(!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo de entrada!\n";
        exit(1);
    }
    cout << "Arquivo de entrada aberto com sucesso!\n";

    // Lê as informações iniciais (a string lixo serve para pular as palavras que não são necessárias para a leitura das variáveis)
    entrada >> lixo >> this->nome;
    entrada >> lixo >> this->qtdDisciplinas;
    entrada >> lixo >> this->qtdSalas;
    entrada >> lixo >> this->qtdLabs;
    entrada >> lixo >> this->qtdDias;
    entrada >> lixo >> this->periodosDia;
    entrada >> lixo >> this->qtdTurnos;
    entrada >> lixo >> this->qtdCurriculos;
    entrada >> lixo >> this->qtdAulasFixas;
    entrada >> lixo >> this->qtdIndisponibilidades;
    entrada >> lixo >> this->qtdPesos;

    // Imprimir todas as variáveis lidas
    cout << "Nome: " << nome << endl;
    cout << "Quantidade de disciplinas: " << qtdDisciplinas << endl;
    cout << "Quantidade de salas: " << qtdSalas << endl;
    cout << "Quantidade de laboratórios: " << qtdLabs << endl;
    cout << "Quantidade de dias: " << qtdDias << endl;
    cout << "Períodos por dia: " << periodosDia << endl;
    cout << "Quantidade de turnos: " << qtdTurnos << endl;
    cout << "Quantidade de currículos: " << qtdCurriculos << endl;
    cout << "Quantidade de aulas fixas: " << qtdAulasFixas << endl;
    cout << "Quantidade de indisponibilidades: " << qtdIndisponibilidades << endl;
    cout << "Quantidade de pesos: " << qtdPesos << endl;


    // Leitura das disciplinas:
    entrada >> lixo;
    for(int i = 0; i < qtdDisciplinas; i++) {
        vector<int> idx_professores;
        string nomeDisciplina, nomeProfessor;
        int qtdProfessores, qtdAulasSemana, numMinimoDiasAula, qtdAlunos, idxProf;
        char salaOuLab;

        entrada >> nomeDisciplina >> qtdProfessores;

        for(int j = 0; j < qtdProfessores; j++) {
            entrada >> nomeProfessor;

            /*  Parte de ver se o professor já existe no array de professores da classe Problema, instanciar novos objetos de professores
                e preencher tanto o vector de professores de dentro da classe Problema, quanto preencher o vector de int de dentro de cada
                disciplina, que contém o indíce do professor correspondente ao vector da classe Problema:
            */
            bool achouProf = false;
            for(int k = 0; k < (int)professores.size(); k++) {
                if(professores[k]->getNomeProfessor() == nomeProfessor) {
                    achouProf = true;
                    idxProf = professores[k]->getIdx();
                    break;
                }
            }
            if(achouProf == false) {
                idxProf = professores.size();
                Professor *p = new Professor(nomeProfessor, idxProf, qtdDias, periodosDia); // Posso fazer isso, pois so entra aqui se o achouProf foi false.
                professores.push_back(p);
            }
            
            idx_professores.push_back(idxProf);
        }
        
        entrada >> qtdAulasSemana >> numMinimoDiasAula >> qtdAlunos >> salaOuLab;

        Disciplina *d = new Disciplina(nomeDisciplina, qtdProfessores, qtdAulasSemana, numMinimoDiasAula, qtdAlunos, i, salaOuLab, qtdDias, periodosDia);
        disciplinas.push_back(d);
        disciplinas[i]->setProfessores(idx_professores);
    }

    // Leitura das salas:
    entrada >> lixo;
    for(int i = 0; i < qtdSalas; i++) {
        string nomeSala;
        int capacidade;

        entrada >> nomeSala >> capacidade;

        Sala *s = new Sala(nomeSala, capacidade, qtdDias, periodosDia, false);
        salas.push_back(s);
    }

    // Ordenação das salas por capacidade (usando função lambda)
    sort(salas.begin(), salas.end(), [](const Sala* a, const Sala* b) {
        return a->getCapacidade() < b->getCapacidade();
    });

    // Leitura dos labs:
    entrada >> lixo;
    for(int i = 0; i < qtdLabs; i++) {
        string nomeLab;
        int capacidade;

        entrada >> nomeLab >> capacidade;

        Sala *s = new Sala(nomeLab, capacidade, qtdDias, periodosDia, true);
        salas.push_back(s);
    }

    // Ordenação das salas de laboratório por capacidade (começando do índice qtdSalas):
    sort(salas.begin() + qtdSalas, salas.end(), [](const Sala* a, const Sala* b) {
        return a->getCapacidade() < b->getCapacidade();
    });

    // Leitura dos turnos:
    entrada >> lixo;
    for(int i = 0; i < qtdTurnos; i++) {
        string nomeTurno;
        int horarioInicio, horarioFim;

        entrada >> nomeTurno >> horarioInicio >> horarioFim;

        Turno *t = new Turno(nomeTurno, horarioInicio, horarioFim);
        this->turnos.push_back(t);
    }

    // Leitura dos currículos:
    entrada >> lixo;
    for(int i = 0; i < qtdCurriculos; i++) {
        string nomeCurriculo, turno;
        int qtdTurnos, qtdDObr, qtdDOpt;
        vector<Turno*> turnosCurriculo;

        entrada >> nomeCurriculo >> qtdTurnos;

        // For para ler os turnos do curriculo:
        for(int j = 0; j < qtdTurnos; j++) {
            string nomeTurno;
            entrada >> nomeTurno;

            // For para buscar o turno correspondente ao nomeTurno (pegar o ponteiro do turno ao invés de ter apenas a string com o nome dele):
            for(int k = 0; k < (int)this->turnos.size(); k++) {
                if(this->turnos[k]->getNomeTurno() == nomeTurno) {
                    turno = nomeTurno;
                    turnosCurriculo.push_back(this->turnos[k]); // Adicionando o turno ao vector de turnos do curriculo.
                    break;
                }
            }
        }

        entrada >> qtdDObr;

        vector<int> disciplinasObrigatorias;
        for(int j = 0; j < qtdDObr; j++) {
            string nomeDisciplina;
            entrada >> nomeDisciplina;
            for(int k = 0; k < (int)this->disciplinas.size(); k++) {
                if(this->disciplinas[k]->getNomeDisciplina() == nomeDisciplina) {
                    disciplinasObrigatorias.push_back(this->disciplinas[k]->getIdx());
                    // Atrelando o turno referente a disciplina obrigatória:
                    if((int) turnosCurriculo.size() == 2) {
                        // Aqui fazemos a escolha de qual dos dois turnos existentes devem ser utilizados para o horário de inicio e qual deve ser usado para o horário de fim:
                        if(turnosCurriculo[0]->getHorarioInicio() < turnosCurriculo[1]->getHorarioInicio()) {
                            Turno *turnoIntegral = new Turno("Int", turnosCurriculo[0]->getHorarioInicio(), turnosCurriculo[1]->getHorarioFim());
                            this->disciplinas[k]->setTurno(turnoIntegral);
                        }
                        else {
                            Turno *turnoIntegral = new Turno("Int", turnosCurriculo[1]->getHorarioInicio(), turnosCurriculo[0]->getHorarioFim());
                            this->disciplinas[k]->setTurno(turnoIntegral);
                        }
                    }
                    else if((int) turnosCurriculo.size() == 1) {    
                        this->disciplinas[k]->setTurno(turnosCurriculo[0]);     
                    }
                    else {
                        cout << "Curriculo possui menos do que um turno ou mais do que dois!\n";
                        exit(1);
                    }
                    break; // Quebra o for de buscar a discplina, pois se entrou aqui, já encontramos ela
                }
            }
        }

        entrada >> qtdDOpt;
        vector<int> disciplinasOptativas;
        for(int j = 0; j < qtdDOpt; j++) {
            string nomeDisciplina;
            entrada >> nomeDisciplina;
            for(int k = 0; k < (int)this->disciplinas.size(); k++) {
                if(this->disciplinas[k]->getNomeDisciplina() == nomeDisciplina) {
                    this->disciplinas[k]->setEhOptativaEmAlgumCurriculo(true);
                    // Atrelando o turno referente a disciplina obrigatória:
                    if((int) turnosCurriculo.size() == 2) {
                        // Aqui fazemos a escolha de qual dos dois turnos existentes devem ser utilizados para o horário de inicio e qual deve ser usado para o horário de fim:
                        if(turnosCurriculo[0]->getHorarioInicio() < turnosCurriculo[1]->getHorarioInicio()) {
                            if(this->disciplinas[k]->getTurnoOptativo() != nullptr) {
                                delete this->disciplinas[k]->getTurnoOptativo();
                            }
                            Turno *turnoIntegral = new Turno("Int", turnosCurriculo[0]->getHorarioInicio(), turnosCurriculo[1]->getHorarioFim());
                            this->disciplinas[k]->setTurnoOptativo(turnoIntegral);
                        }
                        else {
                            if(this->disciplinas[k]->getTurnoOptativo() != nullptr) {
                                delete this->disciplinas[k]->getTurnoOptativo();
                            }
                            Turno *turnoIntegral = new Turno("Int", turnosCurriculo[1]->getHorarioInicio(), turnosCurriculo[0]->getHorarioFim());
                            this->disciplinas[k]->setTurnoOptativo(turnoIntegral);
                        }
                    }
                    else if((int) turnosCurriculo.size() == 1) {    
                        this->disciplinas[k]->setTurnoOptativo(turnosCurriculo[0]);     
                    }
                    else {
                        cout << "Curriculo possui menos do que um turno ou mais do que dois!\n";
                        exit(1);
                    }
                    disciplinasOptativas.push_back(this->disciplinas[k]->getIdx());
                    if(this->disciplinas[k]->getTurno() == nullptr) { // Se a disciplina não tiver um turno atrelado a ela, significa que ela é optativa e precisamos atrelar o turno do currículo a ela.
                        // Atrelando o turno referente a disciplina optativa:
                        if((int) turnosCurriculo.size() == 2) {
                            // Aqui fazemos a escolha de qual dos dois turnos existentes devem ser utilizados para o horário de inicio e qual deve ser usado para o horário de fim:
                            if(turnosCurriculo[0]->getHorarioInicio() < turnosCurriculo[1]->getHorarioInicio()) {
                                Turno *turnoIntegral = new Turno("Int", turnosCurriculo[0]->getHorarioInicio(), turnosCurriculo[1]->getHorarioFim());
                                this->disciplinas[k]->setTurno(turnoIntegral);
                            }
                            else {
                                Turno *turnoIntegral = new Turno("Int", turnosCurriculo[1]->getHorarioInicio(), turnosCurriculo[0]->getHorarioFim());
                                this->disciplinas[k]->setTurno(turnoIntegral);
                            }
                        }
                        else if((int) turnosCurriculo.size() == 1) {    
                            this->disciplinas[k]->setTurno(turnosCurriculo[0]);     
                        }
                        else {
                            cout << "Curriculo possui menos do que um turno ou mais do que dois!\n";
                            exit(1);
                        }
                    }
                    break;
                }
            }
        }

        Curriculo *c = new Curriculo(nomeCurriculo, qtdDObr, qtdDOpt, qtdDias, periodosDia, turnosCurriculo);
        this->curriculos.push_back(c);

        this->curriculos[i]->setDisciplinasObrigatorias(disciplinasObrigatorias);
        this->curriculos[i]->setDisciplinasOptativas(disciplinasOptativas);

        // Atrelando o curriculo com cada uma de suas disciplinas obrigatorias:
        vector<int> disciplinasObr = c->getDisciplinasObrigatorias();
        for(int j = 0; j < qtdDObr; j++) {
            disciplinas[disciplinasObr[j]]->setCurriculo(c);
        }

        // Para disciplinas que não são obrigatórias para nenhum currículo, atrelamos o curriculo em que elas são optativas
        vector<int> disciplinasOpt = c->getDisciplinasOptativas();
        for(int j = 0; j < qtdDOpt; j++) {
            if(disciplinas[disciplinasOpt[j]]->getCurriculo() == nullptr) {
                disciplinas[disciplinasOpt[j]]->setCurriculo(c);
            }
        }
    }

    // Leitura das aulas fixas:
    entrada >> lixo;
    for(int i = 0; i < qtdAulasFixas; i++) {
        string nomeDisciplina;
        int dia, horario;

        entrada >> nomeDisciplina >> dia >> horario;

        for(int j = 0; j < (int)this->disciplinas.size(); j++) {
            if(this->disciplinas[j]->getNomeDisciplina() == nomeDisciplina) {
                this->disciplinas[j]->adicionarAulaFixa(i);
                break;
            }
        }

        AulaFixa *a = new AulaFixa(nomeDisciplina, dia, horario);
        aulasFixas.push_back(a);
    }

    // Leitura das indisponibilidades:
    entrada >> lixo;
    for(int i = 0; i < qtdIndisponibilidades; i++) {
        string nomeDisciplina;
        int dia, horario;

        entrada >> nomeDisciplina >> dia >> horario;

        for(int j = 0; j < (int)this->disciplinas.size(); j++) {
            if(this->disciplinas[j]->getNomeDisciplina() == nomeDisciplina) {
                this->disciplinas[j]->adicionarIndisponibilidade(i);
                break;
            }
        }

        Indisponibilidade *ind = new Indisponibilidade(nomeDisciplina, dia, horario);
        indisponibilidades.push_back(ind);
    }

    // Leituras dos pesos:
    entrada >> lixo;
    for(int i = 0; i < qtdPesos; i++) {
        int peso;
        bool ehForte = false;

        entrada >> peso;

        if(peso == -1) {
            ehForte = true;
        }
        
        Restricao *r = new Restricao(peso, ehForte);
        restricoes.push_back(r);
    }
    
    entrada.close();
}

Problema::~Problema() {
    // GAMBIARRA para deletar turnos Integrais criados:
    for(int i = 0; i < qtdDisciplinas; i++) {
        if(disciplinas[i]->getTurno() != nullptr) {
            if(disciplinas[i]->getTurno()->getNomeTurno() == "Int") {
                delete(disciplinas[i]->getTurno());
            }
        }
    }

    for(int i = 0; i < qtdDisciplinas; i++) {
        delete(disciplinas[i]);
    }

    for(int i = 0; i < qtdSalas + qtdLabs; i++) {
        delete(salas[i]);
    }

    for(int i = 0; i < qtdTurnos; i++) {
        delete(turnos[i]);
    }

    for(int i = 0; i < qtdCurriculos; i++) {
        delete(curriculos[i]);
    }

    for(int i = 0; i < qtdAulasFixas; i++) {
        delete(aulasFixas[i]);
    }

    for(int i = 0; i < qtdIndisponibilidades; i++) {
        delete(indisponibilidades[i]);
    }

    for(int i = 0; i < qtdPesos; i++) {
        delete(restricoes[i]);
    }

    for(int i = 0; i < (int)professores.size(); i++) {
        delete(professores[i]);
    }
}

void Problema::imprimeProblema() {
    cout << "Nome: " << nome << endl;
    cout << "Disciplinas: " << qtdDisciplinas << endl;
    cout << "Salas: " << qtdSalas << endl;
    cout << "Labs: " << qtdLabs << endl;
    cout << "Dias: " << qtdDias << endl;
    cout << "Periodos_por_dia: " << periodosDia << endl;
    cout << "Turnos: " << qtdTurnos << endl;
    cout << "Curriculos: " << qtdCurriculos << endl;
    cout << "Aulas_Fixas: " << qtdAulasFixas << endl;
    cout << "Indisponibilidades: " << qtdIndisponibilidades << endl;
    cout << "Pesos: " << qtdPesos << endl;
    cout << endl;

    cout << "CURSOS:\n";
    for(int i = 0; i < qtdDisciplinas; i++) {
        disciplinas[i]->imprimeDisciplina(this->professores);
    }
    cout << endl;

    cout << "SALAS:\n";
    for(int i = 0; i < qtdSalas; i++) {
        salas[i]->imprimeSala(qtdDias, periodosDia);
    }
    cout << endl;

    cout << "LABS:\n";
    for(int i = qtdSalas; i < qtdSalas + qtdLabs; i++) {
        salas[i]->imprimeSala(qtdDias, periodosDia);
    }
    cout << endl;

    cout << "TURNOS:\n";
    for(int i = 0; i < qtdTurnos; i++) {
        turnos[i]->imprimeTurno();
    }
    cout << endl;

    cout << "CURRICULA:\n";
    for(int i = 0; i < qtdCurriculos; i++) {
        curriculos[i]->imprimeCurriculo(this->disciplinas);
    }
    cout << endl;

    cout << "AULAS_FIXAS:\n";
    for(int i = 0; i < qtdAulasFixas; i++) {
        aulasFixas[i]->imprimeAulaFixa();
    }
    cout << endl;

    cout << "INDISPONIBILIDADES:\n";
    for(int i = 0; i < qtdIndisponibilidades; i++) {
        indisponibilidades[i]->imprimeIndisponibilidade();
    }
    cout << endl;

    cout << "PESOS:\n";
    for(int i = 0; i < qtdPesos; i++) {
        restricoes[i]->imprimeRestricao();
        if(i != qtdPesos - 1) {
            cout << " ";
        }
    }
    cout << endl << endl;

    cout << "FIM.";
}

int Problema::getQtdDisciplinas() {
    return this->qtdDisciplinas;
}

int Problema::getQtdSalas() {
    return this->qtdSalas;
}

int Problema::getQtdLabs() {
    return this->qtdLabs;
}

int Problema::getQtdDias() {
    return this->qtdDias;
}

int Problema::getPeriodosDia() {
    return this->periodosDia;
}

int Problema::getQtdTurnos() {
    return this->qtdTurnos;
}

int Problema::getQtdCurriculos() {
    return this->qtdCurriculos;
}

int Problema::getQtdAulasFixas() {
    return this->qtdAulasFixas;
}

int Problema::getQtdIndisponibilidades() {
    return this->qtdIndisponibilidades;
}

int Problema::getQtdPesos() {
    return this->qtdPesos;
}

vector<AulaFixa*> Problema::getAulasFixas() {
    return this->aulasFixas;
}

vector<Curriculo*> Problema::getCurriculos() {
    return this->curriculos;
}

vector<Disciplina*> Problema::getDisciplinas() {
    return this->disciplinas;
}

vector<Indisponibilidade*> Problema::getIndisponibilidades() {
    return this->indisponibilidades;
}

vector<Restricao*> Problema::getRestricoes() {
    return this->restricoes;
}

vector<Sala*> Problema::getSalas() {
    return this->salas;
}

vector<Turno*> Problema::getTurnos() {
    return this->turnos;
}

vector<Professor*> Problema::getProfessores() {
    return this->professores;
}

string Problema::getNome() {
    return this->nome;
}