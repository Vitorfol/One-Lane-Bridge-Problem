#include "Simulador.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace {

void imprimirUso(const char* nomePrograma) {
    std::cout << "Uso: " << nomePrograma << " [--escala <fator>]\n"
              << "  --escala, -e <fator>   Fator de escala temporal (> 0). Padrao: 1.0 (1s = 1000ms)\n"
              << "  --help, -h             Exibe esta ajuda\n";
}

bool parseEscala(const std::string& texto, double& escalaOut) {
    char* fim = nullptr;
    const double valor = std::strtod(texto.c_str(), &fim);
    if (fim == texto.c_str() || *fim != '\0' || !std::isfinite(valor) || valor <= 0.0) {
        return false;
    }
    escalaOut = valor;
    return true;
}

} // namespace

int main(int argc, char* argv[]) {
    double escala = 1.0;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            imprimirUso(argv[0]);
            return 0;
        }
        if (arg == "--escala" || arg == "-e") {
            if (i + 1 >= argc || !parseEscala(argv[i + 1], escala)) {
                std::cerr << "Erro: informe um fator numerico positivo apos " << arg << ".\n";
                return 1;
            }
            ++i;
            continue;
        }

        std::cerr << "Argumento desconhecido: " << arg << "\n";
        imprimirUso(argv[0]);
        return 1;
    }

    Simulador simulador(escala);
    simulador.executarMenu();
    return 0;
}
