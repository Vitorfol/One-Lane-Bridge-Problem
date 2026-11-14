#include "Simulador.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

Simulador::Simulador(double escalaInicial) : escala(escalaInicial) {}

void Simulador::exibirMenu() const {
    const long long msPorSegundo = std::llround(1000.0 * escala);
    std::cout << "\n============================================================\n"
              << "  Ponte de Faixa Unica  |  Escala: "
              << std::fixed << std::setprecision(2) << escala
              << "x (1s = " << msPorSegundo << "ms)\n"
              << "============================================================\n"
              << "  1. Executar Cenario 1 (Somente carros)\n"
              << "  2. Executar Cenario 2 (Somente carros sem starvation)\n"
              << "  3. Executar Cenario 3 (Carros e caminhoes sem starvation)\n"
              << "  4. Executar Cenario 4 (Carros e caminhoes em duas pontes)\n"
              << "  5. Executar todos os cenarios (1 a 4)\n"
              << "  6. Exibir tabela de Dados da sessao\n"
              << "  7. Alterar escala de tempo\n"
              << "  0. Sair\n"
              << "Escolha uma opcao: ";
}

bool Simulador::lerInteiro(int& valor) const {
    if (std::cin >> valor) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return true;
    }
    if (std::cin.eof()) {
        return false;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    valor = -1;
    return true;
}

void Simulador::alterarEscala() {
    std::cout << "Informe a nova escala (> 0, ex: 1.0 = tempo real, 0.1 = 10x mais rapido): ";
    double novaEscala = 0.0;
    const bool leu = static_cast<bool>(std::cin >> novaEscala);
    if (!std::cin.eof()) {
        if (!leu) {
            std::cin.clear();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (!leu) {
        std::cout << "Valor invalido. Escala mantida em " << escala << "x.\n";
        return;
    }
    if (!std::isfinite(novaEscala) || novaEscala <= 0.0) {
        std::cout << "A escala deve ser maior que zero. Escala mantida em " << escala << "x.\n";
        return;
    }
    escala = novaEscala;
    std::cout << "Escala atualizada para " << std::fixed << std::setprecision(2) << escala << "x.\n";
}

void Simulador::executarCenario(int cenario) {
    std::cout << "\n[Aviso] Cenario " << cenario
              << " ainda nao foi implementado (previsto nos Passos 2 a 5 do backlog).\n";
}

void Simulador::executarMenu() {
    while (true) {
        exibirMenu();
        int opcao = -1;
        if (!lerInteiro(opcao)) {
            std::cout << "\nEntrada encerrada.\n";
            break;
        }

        switch (opcao) {
            case 1:
            case 2:
            case 3:
            case 4:
                executarCenario(opcao);
                break;
            case 5:
                for (int c = 1; c <= TOTAL_CENARIOS; ++c) {
                    executarCenario(c);
                }
                break;
            case 6:
                dados.imprimirTabela(escala);
                break;
            case 7:
                alterarEscala();
                break;
            case 0:
                if (dados.calcularGlobal().rodadas > 0) {
                    dados.imprimirTabela(escala);
                }
                std::cout << "Encerrando simulador.\n";
                return;
            default:
                std::cout << "Opcao invalida. Tente novamente.\n";
                break;
        }
    }
}
