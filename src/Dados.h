#ifndef DADOS_H
#define DADOS_H

#include "Tipos.h"

#include <array>
#include <limits>
#include <mutex>

struct MetricasCenario {
    int rodadas = 0;
    int carrosNS = 0;
    int carrosSN = 0;
    int caminhoesNS = 0;
    int caminhoesSN = 0;

    double somaEsperaSeg = 0.0;
    double minEsperaSeg = std::numeric_limits<double>::infinity();
    double maxEsperaSeg = 0.0;

    double tempoOcupadoPrincipalSeg = 0.0;
    double tempoTotalPrincipalSeg = 0.0;

    double tempoOcupadoAntigaSeg = 0.0;
    double tempoTotalAntigaSeg = 0.0;

    long long totalVeiculos() const {
        return static_cast<long long>(carrosNS) + carrosSN + caminhoesNS + caminhoesSN;
    }

    double esperaMediaSeg() const {
        const long long total = totalVeiculos();
        return (total > 0) ? (somaEsperaSeg / static_cast<double>(total)) : 0.0;
    }

    double utilizacaoPrincipalPct() const {
        return (tempoTotalPrincipalSeg > 0.0)
                   ? (tempoOcupadoPrincipalSeg / tempoTotalPrincipalSeg) * 100.0
                   : 0.0;
    }

    double utilizacaoAntigaPct() const {
        return (tempoTotalAntigaSeg > 0.0)
                   ? (tempoOcupadoAntigaSeg / tempoTotalAntigaSeg) * 100.0
                   : 0.0;
    }
};

class Dados {
private:
    mutable std::mutex mtx;
    std::array<MetricasCenario, TOTAL_CENARIOS> cenarios{};

    static bool cenarioValido(int cenario);
    void acumular(MetricasCenario& destino, const MetricasCenario& origem) const;

public:
    void registrarVeiculo(int cenario, TipoVeiculo tipo, Sentido sentido, double esperaSimuladaSeg);
    void registrarRodada(int cenario,
                         double tempoTotalSeg,
                         double tempoOcupadoPrincipalSeg,
                         double tempoOcupadoAntigaSeg = 0.0,
                         bool possuiPonteAntiga = false);

    MetricasCenario obterCenario(int cenario) const;
    MetricasCenario calcularGlobal() const;
    void imprimirTabela(double escalaAtual) const;
};

#endif
