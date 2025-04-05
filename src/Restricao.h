#pragma once

#include <string>

class Restricao {
    private:
        int peso;
        bool ehForte;
    public:
        Restricao(int peso, bool ehForte);
        int getPeso();
        bool getEhForte();
        void imprimeRestricao();
};