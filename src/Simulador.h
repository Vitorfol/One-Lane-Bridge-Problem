#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Dados.h"

class Simulador {
private:
    Dados dados;
    double escala;

    void exibirMenu() const;
    bool lerInteiro(int& valor) const;
    void alterarEscala();
    void executarCenario(int cenario);

public:
    explicit Simulador(double escalaInicial);
    void executarMenu();
};

#endif
