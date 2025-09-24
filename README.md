# HIBB: Hybrid Interval Branch and Bound

![License: GNU GPLv3](https://img.shields.io/badge/License-GPLv3-blue.svg)

**HIBB (Hybrid Interval Branch and Bound)** é um método de otimização híbrido que integra técnicas de consistência local, típicas de métodos exatos, com uma busca local estocástica baseada em meta-heurísticas.  A abordagem explora o espaço de busca de forma sistemática, evitando a sobreposição de subespaços, uma característica dos métodos de Branch & Bound (B&B).  O HIBB foi desenvolvido como parte da tese de doutorado de Mariane Regina Sponchiado Cassenote no Programa de Pós-Graduação em Informática da Universidade Federal do Paraná (UFPR).

## Sobre o Projeto

O HIBB foi projetado para combinar o rigor dos métodos exatos com a eficiência das meta-heurísticas.  Sua estrutura principal é um algoritmo de B&B intervalar, onde o espaço de busca é iterativamente particionado e avaliado. 

As principais características do HIBB incluem:

* **Estrutura Branch & Bound:** Utiliza um B&B intervalar para segmentar o domínio da instância em caixas (subespaços) que são exploradas sem sobreposição. 
* **Poda com Consistência Local (GAC):** Aplica o contrator GAC (Consistência de Arco Generalizada) em cada caixa para descartar soluções sub-ótimas e infactíveis, tornando a busca mais eficiente. 
* **Busca Local com BBDE:** Emprega o otimizador BBDE para realizar uma busca local estocástica dentro de cada caixa, avaliando sua qualidade e encontrando soluções factíveis que ajudam a podar o espaço de busca global. 
* **Heurísticas de Exploração Avançadas:** Implementa estratégias para guiar a busca, como o uso de múltiplas frentes de exploração (NB) e dois critérios de ordenação para a fila de prioridade: **QPC** (Qualidade do Ponto da Caixa) e **EQPC** (Estagnação + Qualidade do Ponto da Caixa). 

## Como Usar

Siga os passos abaixo para configurar o ambiente, compilar e executar o otimizador HIBB.

### Pré-requisitos

Certifique-se de que você tem as seguintes ferramentas instaladas:
* `git`
* Compilador C (por exemplo, `gcc`)
* `make`

### 1. Estrutura de Diretórios e Instâncias

O HIBB requer um conjunto de instâncias de problemas para sua execução. Estas instâncias e seu processamento de entrada estão disponíveis no repositório [MINLPLib](https://github.com/MSponchiado/MINLPLib.git).

É **essencial** que o diretório do HIBB e o diretório `MINLPLib` estejam localizados no mesmo nível (sejam "irmãos"), como mostra a estrutura abaixo:

seu_diretorio_de_trabalho/
├── HIBB/      <-- Repositório do otimizador
└── MINLPLib/  <-- Repositório das instâncias

### Compilação e Execução

Para compilar e executar o resolvedor em um arquivo de instância, utilize os seguintes comandos:

```bash
./executa.sh <nome_da_instancia>
```

## Licença

Este projeto está licenciado sob a **Licença GNU General Public License v3.0**. Veja o arquivo `LICENSE` para mais detalhes.

## Como Citar Este Trabalho

Se você utilizar o HIBB em sua pesquisa, por favor, cite a seguinte tese:

**Cassenote, M. R. S. (2024).** *Hibridizações entre métodos exatos e meta-heurísticas: Estratégias para otimização global com restrições*. Tese de Doutorado, Programa de Pós-Graduação em Informática, Universidade Federal do Paraná, Curitiba, PR, Brasil.

### BibTeX

```bibtex
@phdthesis{Cassenote2024,
  author  = {Cassenote, Mariane Regina Sponchiado},
  title   = {Hibridizações entre métodos exatos e meta-heurísticas: Estratégias para otimização global com restrições},
  school  = {Universidade Federal do Paraná},
  year    = {2024},
  address = {Curitiba, PR, Brazil},
  month   = {July},
  note    = {Tese de Doutorado em Ciência da Computação}
}
```

## Agradecimentos

Este trabalho foi desenvolvido com o apoio do **Programa de Pós-Graduação em Informática (PPGInf)** da **Universidade Federal do Paraná (UFPR)** e financiado pela **Coordenação de Aperfeiçoamento de Pessoal de Nível Superior (CAPES)**. 

Agradecimentos especiais aos orientadores **Prof. Fabiano Silva** e **Prof. Guilherme Alex Derenievicz** pelo suporte e direcionamento ao longo da pesquisa.
