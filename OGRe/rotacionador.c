/******************************************************************************
 *
 * Copyright (C) 2018 Guilherme Alex Derenievicz
 * This file is part of OGRe
 *
 * OGRe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGRe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGRe.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************


 ******************************************************************************
 *
 *  OGRe - Otimização Global Relaxada
 *
 *  Pré-processador que rotaciona a Árvore Epífita.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "rotacionador_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char **argv) {

  int aux;
  int arcos_insatisfativeis = 0;
  int cabecas_nao_auxiliares = 0;

  no n;
  Var v;
  Rest R;
  PointerRest Pr;

  int nivelBacktracking = 2;
  lista backtrack = cria_lista();

  // char arquivo[1024];
  //int arquivo_count = 1;
  int subdiretorios_count = 1;
  //mkdir("1/", 0777);

  FILE *arq = fopen("instancia_rotacionada.txt", "w");
  //FILE *f = stdout;

  if (argc < 3) {
    printf("Usage: %s <variables_file> <constraints_file> [TIMEOUT [--creates-hypergraph]]\n", argv[0]);
    printf("TIMEOUT               :timeout de busca de rotações melhores segundo algumas heurísticas (ver código-fonte).\n");
    printf("--creates-hypergraph  :(TODO) ao invés de gerar o arquivo de entrada para o OGRe, gera um arquivo .csv do hipergrafo de restrições.\n");
    fclose(arq);
    return 1;
  }

  // ao invés de gerar o arquivo para o
  // solver posso optar por gerar um arquivo
  // csv do grafo para visualização
  int gera_grafo = (argc > 4);

  // le variaveis do 1o arquivo (devolve a raiz)
  lista variaveis = cria_lista();
  char *raiz = le_variaveis(argv[1], variaveis, arq);

  // le restricoes do 2o arquivo
  lista restricoes = cria_lista();
  le_restricoes(argv[2], variaveis, restricoes);
  // começa com a raiz
  n = busca(raiz, variaveis, compara_Var);
  if (!n) {
    printf("ERROR: raiz não encontrada\n");
    return 1;
  }
  v = objeto(n);
  v->nivel = 1;
  atualiza_conjunto_cap(v->rest, '1');

  // timeout de busca de rotações melhores
  long int timeout;
  if (argc > 3)
    timeout = atol(argv[3]);
  else
    timeout = 600; //default
  struct timeval it, st, et;
  gettimeofday(&it, NULL);
  gettimeofday(&st, NULL);
  int otimo_cabecas_nao_auxiliares = 9999999; // TODO
  int primeira_rotacao = 1;

  // começa a busca
  int eh_rotacao = 0;

  gettimeofday(&et , NULL);
  int timed_out = (et.tv_sec - st.tv_sec > timeout)?1:0;

  while (!timed_out && !eh_rotacao && subdiretorios_count <= MAX_DIR_QTD) {

    // Esse if faz sentido ao comentar a condição /*!eh_rotacao*/.
    // Assim, o programa não retorna apenas a primeira Árvore Epífita encontrada, mas
    // várias delas.
    if (eh_rotacao) {

      if (primeira_rotacao) {
        printf("TEMPO 1a ROTACAO: %fs\n", (et.tv_sec - it.tv_sec) + ((double)(et.tv_usec - it.tv_usec))/1000000.0);
        primeira_rotacao = 0;
      }

      imprime_rotacao(backtrack, gera_grafo, arq);

      /*
      sprintf(arquivo, "%d/%d_tmp.txt", subdiretorios_count, arquivo_count++);
      imprime_rotacao(backtrack, gera_grafo, arquivo);

      if (arquivo_count > MAX_FORMULAS_POR_DIR) {
        sprintf(arquivo, "%d/", ++subdiretorios_count);
        if (subdiretorios_count <= MAX_DIR_QTD) {
          mkdir(arquivo, 0777);
          arquivo_count = 1;
        }
      }
      eh_rotacao = 0;
       */

      //printf("%d\n", cabecas_nao_auxiliares);
      otimo_cabecas_nao_auxiliares = cabecas_nao_auxiliares-1;
      gettimeofday(&st, NULL);
      eh_rotacao = 0;

      if (otimo_cabecas_nao_auxiliares == -1) {
        printf("TEMPO TOTAL: %fs\n", (et.tv_sec - it.tv_sec) + ((double)(et.tv_usec - it.tv_usec))/1000000.0);
        return 0;
      }
    }
    else {
      // verifica se ha conflitos ou inferencias
      eh_rotacao = 1;
      n = primeiro_no(restricoes);

      while (n) {
        R = objeto(n);
        if (poda(R, arcos_insatisfativeis, cabecas_nao_auxiliares, otimo_cabecas_nao_auxiliares)) {
          eh_rotacao = 0;
          n = NULL;
        }
        else if (R->cap == 2) {  // inferência
          // Valora X
          if (R->X->nivel == 0) {
            aux = 0;
            R->X->nivel = nivelBacktracking++;
            atualiza_conjunto_cap(R->X->rest, '+');

            if (!igualdade_no_arco(R, 0)) {
              R->X->cabeca_de_arco = 1;
              arcos_insatisfativeis++;
            }
            if (R->X->nome[0] != 'z') {
              R->X->cabeca_de_arco_nao_auxiliar = 1;
              cabecas_nao_auxiliares++;
            }
          }

          // Ou Y
          else if (R->Y->nivel == 0) {
            aux = 1;
            R->Y->nivel = nivelBacktracking++;
            atualiza_conjunto_cap(R->Y->rest, '+');

            if (!igualdade_no_arco(R, 1))  {
              R->Y->cabeca_de_arco = 1;
              arcos_insatisfativeis++;
            }
            if (R->Y->nome[0] != 'z') {
              R->Y->cabeca_de_arco_nao_auxiliar = 1;
              cabecas_nao_auxiliares++;
            }
          }

          // Ou Z
          else {
            aux = 2;
            R->Z->nivel = nivelBacktracking++;
            atualiza_conjunto_cap(R->Z->rest, '+');

            if (!igualdade_no_arco(R, 2))  {
              R->Z->cabeca_de_arco = 1;
              arcos_insatisfativeis++;
            }
            if (R->Z->nome[0] != 'z') {
              R->Z->cabeca_de_arco_nao_auxiliar = 1;
              cabecas_nao_auxiliares++;
            }
          }
          R->ok = 1;

          // e bota na pilha de backtrack
          Pr = (PointerRest) malloc(sizeof(t_PointerRest));
          Pr->ja_buscada = 1;
          Pr->rotacao = aux;
          Pr->rest = R;
          Pr->tipo = 1;
          empilha(Pr, backtrack);

          // recomeça o processo
          n = primeiro_no(restricoes);
        }
        else
          n = proximo_no(n);

      }
    }

    // Se nenhum conflito foi encontrado
    // empilhamos todas as rotações possíveis
    // para fazer o backtracking
    // ATENCAO: só vou empilhar restrições
    // com uma variável já valorada. Isso pode
    // deixar o algoritmo "incompleto".
    if (eh_rotacao)
      for (n = primeiro_no(restricoes); n; n = proximo_no(n)) {
        R = objeto(n);
        if (R->cap == 1) {
          Pr = (PointerRest) malloc(sizeof(t_PointerRest));
          Pr->rest = R;
          Pr->ja_buscada = 0;
          if (R->X->nivel != 0)
            Pr->rotacao = 0;
          else if (R->Y->nivel != 0)
            Pr->rotacao = 1;
          else
            Pr->rotacao = 2;

          Pr->tipo = 2;
          empilha(Pr, backtrack);
          // não é rotação, pois essa restrição não foi buscada
          eh_rotacao = 0;
        }
      }

    // Se houve conflito ou nem todas foram buscadas, temos que fazer um retrocesso
    if (!eh_rotacao) {

      // Procura uma restrição que ainda não foi buscada
      // da pilha de backtrack
      Pr = objeto(primeiro_no(backtrack));
      while (Pr->ja_buscada) {

        // Desfaz valorações de níveis anteriores (inferências, etc).
        nivelBacktracking--;
        for (n = primeiro_no(variaveis); n; n = proximo_no(n)) {
          v = objeto(n);

          if (v->nivel == nivelBacktracking) {
            if (v->cabeca_de_arco) arcos_insatisfativeis--;
            if (v->cabeca_de_arco_nao_auxiliar) cabecas_nao_auxiliares--;
            v->nivel = 0;
            v->cabeca_de_arco = 0;
            v->cabeca_de_arco_nao_auxiliar = 0;
            atualiza_conjunto_cap(v->rest, '-');
          }
        }

        // remove caso do backtrack
        remove_no(primeiro_no(backtrack), backtrack, destroi_PointerRest);
        if (!primeiro_no(backtrack)) {
          printf("TEMPO TOTAL: %fs\n", (et.tv_sec - it.tv_sec) + ((double)(et.tv_usec - it.tv_usec))/1000000.0);
          printf("erro no backtrack\n");
          return 1;
        }
        else
          Pr = objeto(primeiro_no(backtrack));
      }

      // Faz a busca nessa próxima restrição,
      // refazendo todo o processo.
      R = Pr->rest;
      if (R->X->nivel == 0) {
        R->X->nivel = nivelBacktracking;
        atualiza_conjunto_cap(R->X->rest, '+');
      }
      if (R->Y->nivel == 0) {
        R->Y->nivel = nivelBacktracking;
        atualiza_conjunto_cap(R->Y->rest, '+');
      }
      if (R->Z->nivel == 0) {
        R->Z->nivel = nivelBacktracking;
        atualiza_conjunto_cap(R->Z->rest, '+');
      }
      R->ok = 1;
      Pr->ja_buscada = 1;
      nivelBacktracking++;
    }

    gettimeofday(&et , NULL);
    timed_out = (et.tv_sec - st.tv_sec > timeout)?1:0;
    if (timed_out)
      printf("TIMEOUT: %s\n", argv[1]);
  }

  if (eh_rotacao) {
    imprime_rotacao(backtrack, gera_grafo, arq);
  }
  // COMENTAR UMA LINHA CASO QUEIRA MELHOR ROTACAO
  //printf("TEMPO TOTAL: %fs\n", (et.tv_sec - it.tv_sec) + ((double)(et.tv_usec - it.tv_usec))/1000000.0);
  //printf("TEMPO TOTAL: %fs\n", (et.tv_sec - it.tv_sec) + ((double)(et.tv_usec - it.tv_usec))/1000000.0);

  fclose(arq);
  return 0;
}
