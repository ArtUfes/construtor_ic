#pragma once

#include <string>
#include <vector>

class Disciplina;
class TSFO;
class Restricao;
class Curriculo;
class Alocacao;
class Professor;

/*
    Restrições soft:

    - S1 Disciplinas optativas devem estar preferencialmente no turno do curso
    - S2 Deve-se procurar manter as aulas de uma disciplina na mesma sala
    - S3 Deve-se procurar manter as aulas de uma disciplina no mesmo horário
    - S4 Disciplinas de um mesmo currículo (turma) devem ter horários adjacentes
    - S5 Disciplinas de mesmo professor devem ter horários adjacentes
    - S6 Deve-se impedir sobrecargas de horários para professores que dão aula em turno noturno, por exemplo não poderá dar aula no turno noturno em um dia e no outro no turno matutino, não permitir aulas nos três turnos em um mesmo dia e também só ter aula no matutino e noturno, deixando o vespertino vazio.
*/

class VariacaoFO {
    private:
        int valor;
    public:
        VariacaoFO();
        int getValor();

        static int calculaVariacaoTurnoOptativa(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, bool debug);
        static int calculaVariacaoAulaMesmaSala(Disciplina *d, std::vector<Alocacao*> aulasAlocadas, TSFO *tsfo, std::vector<Restricao*> restricoes, bool debug);
        static int calculaVariacaoAulaMesmoHorario(Disciplina *d, std::vector<Alocacao*> aulasAlocadas, TSFO *tsfo, std::vector<Restricao*> restricoes, bool debug);
        static int calculaVariacaoAulaCurriculoAdjacente(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, bool debug);
        static int calculaVariacaoAulaProfessorAdjacente(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, std::vector<Professor*> professores, bool debug);
        static int calculaVariacaoSobrecargaProfessor(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, std::vector<Professor*> professores, bool debug);

        static int calculaVariacaoTotal(Disciplina *d, TSFO *tsfo, std::vector<Restricao*> restricoes, std::vector<Alocacao*> aulasAlocadas, std::vector<Professor*> professores, bool debug);
};