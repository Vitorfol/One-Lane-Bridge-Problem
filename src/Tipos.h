#ifndef TIPOS_H
#define TIPOS_H

#include <chrono>
#include <cmath>
#include <string>

enum class Sentido {
    NorteSul = 0,
    SulNorte = 1
};

enum class TipoVeiculo {
    Carro,
    Caminhao
};

constexpr int TOTAL_CENARIOS = 4;

inline std::string sentidoParaTexto(Sentido sentido) {
    return (sentido == Sentido::NorteSul) ? "N->S" : "S->N";
}

inline Sentido sentidoOposto(Sentido sentido) {
    return (sentido == Sentido::NorteSul) ? Sentido::SulNorte : Sentido::NorteSul;
}

inline std::chrono::milliseconds segundosParaMs(double segundosSimulados, double escala) {
    const long long ms = static_cast<long long>(std::llround(segundosSimulados * 1000.0 * escala));
    return std::chrono::milliseconds(ms < 0 ? 0 : ms);
}

inline double msReaisParaSegundosSimulados(double msReais, double escala) {
    return (msReais / 1000.0) / escala;
}

#endif
