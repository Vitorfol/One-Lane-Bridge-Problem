# Diretrizes do Projeto — Ponte de Faixa Única (C++)

## 1. Fluxo de Trabalho e Tomada de Decisão
- **Passo a passo:** Implementar, compilar, testar e revisar **uma etapa/questão por vez**, seguindo o `docs/backlog.md`. Nunca implementar todo o projeto de uma só vez.
- **Papéis:** O usuário toma as decisões e valida; o agente propõe, aponta erros ou alternativas tecnicamente superiores e implementa após alinhamento.
- **Comunicação obrigatória de desvios:** Sempre comunicar imediatamente ao usuário qualquer desalinhamento com o enunciado, ambiguidade, risco técnico ou ponto de decisão — mesmo que uma correção seja óbvia.

## 2. Estilo de Comunicação
- Responder de forma direta, objetiva e sem rodeios ou elogios gratuitos.
- Focar na contribuição técnica para resolver o problema e apoiar o aprendizado do usuário sobre concorrência em C++.

## 3. Padrão de Código e Organização
- Código em C++17 limpo, legível, modularizado e sem duplicação.
- Cada trecho de código deve atender diretamente a um requisito de `problema/problema.md`. Evitar código especulativo ou desnecessário.
- Usar `problema/problema.md` como referência principal do enunciado (não há necessidade de ler o PDF).
- Manter o repositório limpo com estrutura semântica (`src/`, `docs/`, `problema/`, `.agents/`, `Makefile`).
- Atualizar `docs/backlog.md` e `docs/arquitetura.md` conforme cada etapa for concluída.
