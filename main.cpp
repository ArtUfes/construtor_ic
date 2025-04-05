#include <iostream>
#include <iomanip>
#include "src/Problema.h"
#include "src/Solucao.h"

#include <vector>
#include "src/Disciplina.h"
#include "src/Professor.h"
#include "src/Curriculo.h"


using namespace std;

int main(int argc, char* argv[]) {
    Problema *p = new Problema(argv[1]);

    // const int larguraNomeDisciplina = 15;

    // imprimir nome de disciplinas e a variavel se ela é optativa em algum currículo:
    // vector<Disciplina*> disciplinas = p->getDisciplinas();
    // cout << "Disciplinas e se são optativas em algum currículo:\n\n";
    // cout << "DISCIPLINA      - EH OPTATIVA?\n";
    // for(int i = 0; i < (int) disciplinas.size(); i++) {
    //     cout << left << setw(larguraNomeDisciplina) << disciplinas[i]->getNomeDisciplina() << " - " << (disciplinas[i]->getEhOptativaEmAlgumCurriculo() ? "SIM" : "NÃO") << endl;
    // }

    // exit(1);



    Solucao *s = new Solucao();
    
    s->geraListaAulasNaoAlocadas(p);
    s->imprimeListaAulasNaoAlocadas();
    
    s->geraListaAulasAlocadas(p);
    s->imprimeListaAulasNaoAlocadas();
    
    // p->imprimeProblema();

    s->imprimeListaAulasAlocadas();

    // vector<Professor*> professores = p->getProfessores();
    // for(int i = 0; i < (int) professores.size(); i++) {
    //     professores[i]->imprimirMatrizDiaHorarioDisponiveis();
    // }

    // cout << "\n\n";
    // vector<Disciplina*> disciplinas = p->getDisciplinas();
    // for(int i = 0; i < (int) disciplinas.size(); i++) {
    //     disciplinas[i]->imprimirMatrizDiaHorarioDisponiveis();
    // }

    // // Imprimir as matrizes de disponibilidades de timeSlots dos curriculos:
    // vector<Curriculo*> curriculos = p->getCurriculos();

    // for(int i = 0; i < p->getQtdCurriculos(); i++) {
    //     curriculos[i]->imprimirMatrizDiaHorarioDisponiveis();
    // }



    delete(s);
    
    delete(p);

    return 0;
}