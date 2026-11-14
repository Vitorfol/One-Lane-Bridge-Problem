# Contexto e Especificação Técnica: Problema da Ponte de Faixa Única (C++ Concorrente)

Este documento sintetiza a discussão, escolhas arquiteturais e a implementação final desenvolvidas para resolver o problema clássico da **Ponte de Faixa Única** utilizando o paradigma de **Monitores** em C++ moderno (C++11 ou superior).

---

## 1. Primitivas de Concorrência em C++ Utilizadas
Diferente do Java (onde qualquer objeto possui sincronização embutida), o C++ adota uma filosofia modular baseada na STL (`<mutex>`, `<condition_variable>`, `<thread>`):

- **`std::thread`**: Representa uma linha de execução nativa do sistema operacional.
- **`std::mutex`**: Mecanismo básico de exclusão mútua (`lock`/`unlock`).
- **`std::unique_lock<std::mutex>`**: Wrapper baseado em RAII (libera o lock automaticamente ao sair do escopo) e **obrigatório** para o uso com variáveis de condição.
- **`std::condition_variable`**: Mecanismo de espera e notificação (equivalente ao modelo de monitores).
  - `cv.wait(lock, predicado)`: Libera o lock atômicamente, coloca a thread para dormir e valida o predicado ao acordar (protegendo contra *spurious wakeups*).
  - `cv.notify_all()`: Acorda todas as threads em espera para reavaliar o estado de forma justa.

---

## 2. Diagnóstico do Problema e Solução de Starvation (Equidade por Lotes)
- **O Problema Ingenuo:** Permitir que carros da mesma direção passem indefinidamente sem verificar o outro lado causa *starvation* (inanição) no sentido oposto.
- **A Solução (Batching + Contadores):** 
  - Mantemos contadores de espera (`esperando[2]`).
  - O predicado de entrada valida:
    1. Se a ponte está vazia (`carrosNaPonte == 0`), entra imediatamente.
    2. Se já está indo na mesma direção (`direcaoAtual == direcao`), só entra **se não houver carros do sentido oposto esperando** (`esperando[oposta] == 0`). Isso garante lotes eficientes sem sacrificar a equidade.

---

## 3. Arquitetura Orientada a Objetos
O sistema foi estruturado em três componentes principais:
1. **`Ponte` (O Monitor):** Encapsula o estado compartilhado (`carrosNaPonte`, `direcaoAtual`, `esperando`), o mutex, a condition variable e as regras de negócio (predicado).
2. **`Carro` (A Entidade):** Encapsula os atributos do veículo (`id`, `direcao`, `tempoTravessiaMs`) e o comportamento executado concorrentemente (`rodar`).
3. **`Main` (O Orquestrador):** Implementa uma simulação assíncrona baseada em tempo real utilizando geradores de números aleatórios modernos do C++ (`std::mt19937`).

---

## 4. Código Fonte Final Implementado

```cpp
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <vector>
#include <random>

// Mutex global para evitar o embaralhamento de prints no terminal
std::mutex coutMtx;
void logSeguro(const std::string& mensagem) {
    std::lock_guard<std::mutex> lock(coutMtx);
    std::cout << mensagem << "\n";
}

// ==========================================
// 1. O MONITOR (A Ponte)
// ==========================================
class Ponte {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int carrosNaPonte = 0;
    int direcaoAtual = -1; // -1: vazia, 0: N->S, 1: S->N
    int esperando[2] = {0, 0};

    std::string direcaoStr(int dir) {
        return (dir == 0) ? "N->S" : "S->N";
    }

public:
    void entrar(int direcao, int id) {
        std::unique_lock<std::mutex> lock(mtx);
        int oposta = 1 - direcao;
        esperando[direcao]++;

        // Predicado de controle com prevenção de starvation
        cv.wait(lock, [this, direcao, oposta]() {
            if (carrosNaPonte == 0) return true;
            return (direcaoAtual == direcao) && (esperando[oposta] == 0);
        });

        esperando[direcao]--;
        direcaoAtual = direcao;
        carrosNaPonte++;
        
        logSeguro("🚗 Carro " + std::to_string(id) + " [" + direcaoStr(direcao) + 
                  "] ENTROU. (Na ponte: " + std::to_string(carrosNaPonte) + ")");
    }

    void sair(int direcao, int id) {
        std::unique_lock<std::mutex> lock(mtx);
        carrosNaPonte--;
        
        logSeguro("💨 Carro " + std::to_string(id) + " [" + direcaoStr(direcao) + 
                  "] SAIU. (Na ponte: " + std::to_string(carrosNaPonte) + ")");

        if (carrosNaPonte == 0) {
            direcaoAtual = -1;
            cv.notify_all(); // Libera todas as threads para reavaliar o acesso
        }
    }
};

// ==========================================
// 2. A CLASSE CARRO
// ==========================================
class Carro {
private:
    int id;
    int direcao;
    int tempoTravessiaMs;

public:
    Carro(int id, int direcao, int tempoTravessiaMs) 
        : id(id), direcao(direcao), tempoTravessiaMs(tempoTravessiaMs) {}

    void rodar(Ponte& ponte) const {
        ponte.entrar(direcao, id);
        std::this_thread::sleep_for(std::chrono::milliseconds(tempoTravessiaMs));
        ponte.sair(direcao, id);
    }
};

// ==========================================
// 3. O ORQUESTRADOR (Simulador)
// ==========================================
int main() {
    Ponte ponte;
    std::vector<std::thread> threadsCarros;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disDirecao(0, 1);
    std::uniform_int_distribution<> disChegada(80, 250);   
    std::uniform_int_distribution<> disTravessia(150, 400); 

    std::cout << "=== INICIANDO SIMULACAO DA PONTE (5 segundos) ===\n\n";
    auto inicio = std::chrono::steady_clock::now();
    int idCarro = 1;

    while (true) {
        auto agora = std::chrono::steady_clock::now();
        if (std::chrono::duration<double>(agora - inicio).count() > 5.0) break;

        int dir = disDirecao(gen);
        int tempoTravessia = disTravessia(gen);

        Carro carro(idCarro++, dir, tempoTravessia);
        threadsCarros.emplace_back(&Carro::rodar, carro, std::ref(ponte));

        std::this_thread::sleep_for(std::chrono::milliseconds(disChegada(gen)));
    }

    std::cout << "\n[Fim da geracao. Aguardando ultimos carros completarem a travessia...]\n";
    for (auto& t : threadsCarros) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "\n=== SIMULACAO ENCERRADA COM SUCESSO ===\n";
    return 0;
}
```

---

## 5. Próximos Passos (Future Steps)
- Compilar e executar o código utilizando um compilador com suporte a C++11 ou superior (ex: `g++ -std=c++11 -pthread main.cpp -o ponte`).
- Ajustar os intervalos de tempo (`disChegada` e `disTravessia`) para testar o comportamento sob alta e baixa concorrência.