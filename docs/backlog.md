# Backlog — Problema da Ponte de Faixa Única

Referência do enunciado: [problema.md](../problema/problema.md)

## Status Geral
- [x] **Passo 0 — Setup e Organização do Projeto**
  - Estrutura de pastas (`src/`, `docs/`, `.agents/`), `Makefile` com `g++ -pthread`, diretrizes e documentação inicial.

- [ ] **Passo 1 — Fundação: Menu Interativo, Escala de Tempo e Classe `Dados`**
  - CLI aceita `--escala <fator>` opcional (padrão `1.0`, `> 0`) + menu interativo em loop na execução do binário (rodar cenário 1..4, rodar todos, ajustar escala, exibir tabela, sair).
  - Classe `Dados` mantém o histórico da sessão em memória (reinicia ao reiniciar o binário), contando o número de `rodadas` de cada cenário e consolidando a coluna `Global` em tabela:
    1. Rodadas executadas por cenário.
    2. Quantidade de carros e caminhões por sentido (`N->S` e `S->N`).
    3. Tempo mínimo, máximo e médio de espera na fila (em segundos simulados).
    4. Taxa de utilização da Ponte Principal e da Ponte Antiga (`tempo com >= 1 veículo / tempo total`).

- [ ] **Passo 2 — Etapa 1: Somente Carros**
  - `n = 100` carros (`50` em cada sentido), `Ta = 1..3s`, `Tc = 5s`, `Ts = 1s` (máximo 5 carros simultâneos).
  - Mesmo sentido pode entrar respeitando `Ts = 1s`; sentido oposto aguarda a ponte esvaziar.
  - Compilar, executar em escala rápida, validar ausência de deadlock e conferir saída da tabela `Dados`.

- [ ] **Passo 3 — Etapa 2: Somente Carros sem Starvation**
  - `n = 100` (`50` por sentido), `Ta = 1..3s`, `Tc = 5s`, `Ts = 1s`, limite de lote `P = 5`.
  - Alternância de sentido após `P = 5` veículos consecutivos quando houver veículos aguardando no sentido oposto, sem sinalizador externo (decisão via estado compartilhado no Monitor).
  - Validar ausência de starvation e deadlock.

- [ ] **Passo 4 — Etapa 3: Carros e Caminhões sem Starvation**
  - `n = 100` carros (`50`/lado) e `m = 10` caminhões (`5`/lado), `Ta = 1..3s`, `Tc = 5s`, `Tt = 10s`, `Ts = 1s`.
  - Classe `Caminhao`: travessia exclusiva (espera ponte vazia; ninguém entra enquanto atravessa).
  - Alternância de sentido a cada `P = 5` carros ou `U = 1` caminhão quando houver espera oposta.

- [ ] **Passo 5 — Etapa 4: Carros e Caminhões em Duas Pontes**
  - `n = 120` carros (`60`/lado) e `m = 10` caminhões (`5`/lado).
  - Segunda ponte (Antiga): apenas carros, `Q = 2` simultâneos, `Ts' = 2s`, `Tc' = 4s`.
  - Fila única por sentido encaminhando carros para a ponte antiga quando a principal estiver ocupada, visando otimizar o tempo total de escoamento.

- [ ] **Passo 6 — Documentação Técnica Final**
  - Atualizar `docs/arquitetura.md` com o diagrama de classes completo (Mermaid) e detalhamento final das classes, métodos e primitivas de sincronização.

- [ ] **Passo 7 — Preparação de Entrega (Relatório e Vídeo Demo)**
  - Execução completa em escala real (`1.0`) para coleta dos resultados finais.
  - Roteiro curto para gravação do vídeo de demonstração e apoio aos tópicos do relatório escrito.
