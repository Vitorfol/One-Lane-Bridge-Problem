#include "Dados.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

std::string formatarInteiro(int valor) {
    return std::to_string(valor);
}

std::string formatarDecimal(double valor, bool disponivel, const std::string& sufixo = "") {
    if (!disponivel) {
        return "-";
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << valor << sufixo;
    return oss.str();
}

void imprimirLinhaTabela(const std::string& rotulo, const std::array<std::string, 5>& colunas) {
    std::cout << "| " << std::left << std::setw(22) << rotulo;
    for (const auto& col : colunas) {
        std::cout << " | " << std::right << std::setw(10) << col;
    }
    std::cout << " |\n";
}

void imprimirSeparadorTabela() {
    std::cout << "+------------------------+------------+------------+------------+------------+------------+\n";
}

} // namespace

bool Dados::cenarioValido(int cenario) {
    return cenario >= 1 && cenario <= TOTAL_CENARIOS;
}

void Dados::registrarVeiculo(int cenario, TipoVeiculo tipo, Sentido sentido, double esperaSimuladaSeg) {
    if (!cenarioValido(cenario)) {
        return;
    }

    std::lock_guard<std::mutex> lock(mtx);
    MetricasCenario& m = cenarios[static_cast<std::size_t>(cenario - 1)];

    if (tipo == TipoVeiculo::Carro) {
        if (sentido == Sentido::NorteSul) {
            m.carrosNS++;
        } else {
            m.carrosSN++;
        }
    } else {
        if (sentido == Sentido::NorteSul) {
            m.caminhoesNS++;
        } else {
            m.caminhoesSN++;
        }
    }

    const double espera = std::max(0.0, esperaSimuladaSeg);
    m.somaEsperaSeg += espera;
    m.minEsperaSeg = std::min(m.minEsperaSeg, espera);
    m.maxEsperaSeg = std::max(m.maxEsperaSeg, espera);
}

void Dados::registrarRodada(int cenario,
                            double tempoTotalSeg,
                            double tempoOcupadoPrincipalSeg,
                            double tempoOcupadoAntigaSeg,
                            bool possuiPonteAntiga) {
    if (!cenarioValido(cenario)) {
        return;
    }

    std::lock_guard<std::mutex> lock(mtx);
    MetricasCenario& m = cenarios[static_cast<std::size_t>(cenario - 1)];
    m.rodadas++;

    const double total = std::max(0.0, tempoTotalSeg);
    m.tempoTotalPrincipalSeg += total;
    m.tempoOcupadoPrincipalSeg += std::clamp(tempoOcupadoPrincipalSeg, 0.0, total);

    if (possuiPonteAntiga) {
        m.tempoTotalAntigaSeg += total;
        m.tempoOcupadoAntigaSeg += std::clamp(tempoOcupadoAntigaSeg, 0.0, total);
    }
}

void Dados::acumular(MetricasCenario& destino, const MetricasCenario& origem) const {
    destino.rodadas += origem.rodadas;
    destino.carrosNS += origem.carrosNS;
    destino.carrosSN += origem.carrosSN;
    destino.caminhoesNS += origem.caminhoesNS;
    destino.caminhoesSN += origem.caminhoesSN;

    destino.somaEsperaSeg += origem.somaEsperaSeg;
    if (origem.totalVeiculos() > 0) {
        destino.minEsperaSeg = std::min(destino.minEsperaSeg, origem.minEsperaSeg);
        destino.maxEsperaSeg = std::max(destino.maxEsperaSeg, origem.maxEsperaSeg);
    }

    destino.tempoOcupadoPrincipalSeg += origem.tempoOcupadoPrincipalSeg;
    destino.tempoTotalPrincipalSeg += origem.tempoTotalPrincipalSeg;
    destino.tempoOcupadoAntigaSeg += origem.tempoOcupadoAntigaSeg;
    destino.tempoTotalAntigaSeg += origem.tempoTotalAntigaSeg;
}

MetricasCenario Dados::obterCenario(int cenario) const {
    if (!cenarioValido(cenario)) {
        return MetricasCenario{};
    }
    std::lock_guard<std::mutex> lock(mtx);
    return cenarios[static_cast<std::size_t>(cenario - 1)];
}

MetricasCenario Dados::calcularGlobal() const {
    std::lock_guard<std::mutex> lock(mtx);
    MetricasCenario global;
    for (const auto& m : cenarios) {
        acumular(global, m);
    }
    return global;
}

void Dados::imprimirTabela(double escalaAtual) const {
    std::array<MetricasCenario, 5> cols{};
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (int i = 0; i < TOTAL_CENARIOS; ++i) {
            cols[static_cast<std::size_t>(i)] = cenarios[static_cast<std::size_t>(i)];
            acumular(cols[4], cenarios[static_cast<std::size_t>(i)]);
        }
    }

    std::cout << "\n=== TABELA DE DADOS DA SESSAO (tempos em segundos simulados | escala: "
              << std::fixed << std::setprecision(2) << escalaAtual << "x) ===\n";

    imprimirSeparadorTabela();
    imprimirLinhaTabela("Metrica", {"Cen. 1", "Cen. 2", "Cen. 3", "Cen. 4", "Global"});
    imprimirSeparadorTabela();

    auto preencher = [&](auto extrator) {
        std::array<std::string, 5> linha{};
        for (std::size_t i = 0; i < 5; ++i) {
            linha[i] = extrator(cols[i], i);
        }
        return linha;
    };

    imprimirLinhaTabela("Rodadas", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarInteiro(m.rodadas);
    }));
    imprimirLinhaTabela("Carros N->S", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarInteiro(m.carrosNS);
    }));
    imprimirLinhaTabela("Carros S->N", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarInteiro(m.carrosSN);
    }));
    imprimirLinhaTabela("Caminhoes N->S", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarInteiro(m.caminhoesNS);
    }));
    imprimirLinhaTabela("Caminhoes S->N", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarInteiro(m.caminhoesSN);
    }));

    imprimirSeparadorTabela();

    imprimirLinhaTabela("Espera Min (s)", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarDecimal(m.minEsperaSeg, m.totalVeiculos() > 0);
    }));
    imprimirLinhaTabela("Espera Med (s)", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarDecimal(m.esperaMediaSeg(), m.totalVeiculos() > 0);
    }));
    imprimirLinhaTabela("Espera Max (s)", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarDecimal(m.maxEsperaSeg, m.totalVeiculos() > 0);
    }));

    imprimirSeparadorTabela();

    imprimirLinhaTabela("Uso Principal (%)", preencher([](const MetricasCenario& m, std::size_t) {
        return formatarDecimal(m.utilizacaoPrincipalPct(), m.tempoTotalPrincipalSeg > 0.0, "%");
    }));
    imprimirLinhaTabela("Uso Antiga (%)", preencher([](const MetricasCenario& m, std::size_t idx) {
        const bool aplicavel = (idx == 3 || idx == 4) && (m.tempoTotalAntigaSeg > 0.0);
        return formatarDecimal(m.utilizacaoAntigaPct(), aplicavel, "%");
    }));

    imprimirSeparadorTabela();
}
