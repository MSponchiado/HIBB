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
 *  Branch and Bound intervalar (RAC direcionada aproximada).
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "branch-bound.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

long int TIMEOUT = 7200;
unsigned int DEBUG = FALSE;
unsigned int FULL_CONSISTENCY = TRUE;

//--------------------------------------------------------------------------

ramo cria_ramo(Variavel V, Mintervalo M, ramo p, int conflit, epifita C) {

  ramo r = (ramo) malloc(sizeof(t_ramo));
  if (r) {
    r->valoracao = cria_lista();

    if (!r->valoracao) {
      free(r);
      return NULL;
    }

    r->var_decidida = V;
    r->valor_decidido = M;
    r->pai = p;
    r->conflito = conflit;
    r->nivel = (p ? nivel(p)+1 : 0);
    r->jah_visitado = FALSE;
    salva(C, r);
  }
  else
    printf("ERRO: Impossível criar ramo\n");

  return r;
}

//--------------------------------------------------------------------------
void destroi_ramo(void *r) {

  destroi_lista(valoracao(r), destroi_Mintervalo);
  destroi_Mintervalo(valor_decidido(r));

  free(r);
}

//--------------------------------------------------------------------------

lista valoracao(ramo r) { return r->valoracao; }

Variavel var_decidida(ramo r) { return r->var_decidida; }

Mintervalo valor_decidido(ramo r) { return r->valor_decidido; }

int jah_visitado(ramo r) { return r->jah_visitado; }

ramo ramo_pai(ramo r) { return r->pai; }

int conflito(ramo r) { return r->conflito; }

int nivel(ramo r) { return r->nivel; }

//--------------------------------------------------------------------------


// atualiza a decomposição epífita C com os multi-intervalos da lista val:
// aplica a valoração na Árvore Epífita (seguindo a ordem das variáveis)
// *copiando* os multi-intervalos da lista val para a epifita C.
void aplica(epifita C, lista val) {

  no l = primeiro_no(val);

  for (no m = primeiro_no(arvores(C)); m; m = proximo_no(m))
    for (no n = primeiro_no(variaveis(objeto(m))); n; n = proximo_no(n)) {

      Variavel X = objeto(n);

      destroi_Mintervalo(multi_intervalo(X));
      X->multi_intervalo = copia_Mintervalo(objeto(l));

      l = proximo_no(l);
    }
}

//--------------------------------------------------------------------------

// salva os multi-intervalos da decomposição epífita C no ramo r, *copiando* os multi-intervalos.
// nota-se que a lista de valoracao do ramo r é *limpada*, sendo possível reaproveitar
// um ramo.
void salva(epifita C, ramo r) {

  limpa_lista(valoracao(r), destroi_Mintervalo);

  for (no m = primeiro_no(arvores(C)); m; m = proximo_no(m))
    for (no n = primeiro_no(variaveis(objeto(m))); n; n = proximo_no(n)) {

      Variavel X = objeto(n);

      enfileira(copia_Mintervalo(multi_intervalo(X)), valoracao(r));
    }
}

//--------------------------------------------------------------------------

// Cria um ramo filho do ramo atual no qual X foi particionado e todas as inferências feitas.
// O lado pode ser 'a', 'b' ou 'n', onde o última indica que não se deve
// particionar X.
ramo ramifica(Variavel X, restricao R, char lado, ramo atual, epifita C) {

  if (lado != 'n')
    M_particiona(multi_intervalo(X), lado, DELTA);

  Mintervalo valor = copia_Mintervalo(multi_intervalo(X));

  lista vars = cria_lista();
  enfileira(X, vars);

  int conflit;
  if (FULL_CONSISTENCY)
    conflit = global_consistency(C, vars);
  else
    conflit = directional_consistency(C, busca(R, complete_odv(C), compara_restricao), 1);

  ramo r = cria_ramo(X, valor, atual, conflit, C);
  return r;
}

//--------------------------------------------------------------------------

// Casca para a busca local. Aqui eu faço esse "upper-bound" que (ATENÇÃO) não valora as variáveis!
// monta a pilha de insatisfativeis com restrições não RAC-consistentes
int tem_solucao(epifita C, lista insatisfativeis) {

  // valor objetivo
  Mintervalo M = multi_intervalo(var_objetivo(C));
  double value = i_min(objeto(primeiro_no(M)));

  if (!eh_minfinito(value) && r_cmp(max_erro_RAC(C), "<", ERRO_RAC, NULL)) {

    // atribui valor minimo ao intervalo objetivo
    limpa_Mintervalo(M);
    enfileira(cria_intervalo(1, 1, value, value), M);

    return TRUE;
  }

  return busca_local(C, insatisfativeis);
}

//--------------------------------------------------------------------------

// aplica o valor_objetivo como upper_bound na valoração do ramo_atual.
void upper_bound(double valor_objetivo, ramo ramo_atual, epifita C) {

  if (conflito(ramo_atual))
    return;

  // percorre o multi-intervalo objetivo de trás para frente, removendo os intervalos [a,b] que têm
  // a > valor_objetivo e mudando para [a, valor_objetivo) os intervalos que têm a < valor_objetivo <= b.
  Variavel OBJ = var_objetivo(C);
  unsigned int OBJ_alterado = FALSE;
  no m = ultimo_no(multi_intervalo(OBJ));
  intervalo Im; // Im = objeto(m)
  while (m && (Im = objeto(m)) && r_cmp(Im->b, ">=", valor_objetivo, &(Im->b)))
    if (r_cmp(Im->a, "<", valor_objetivo, &(Im->a))) {
      Im->b = valor_objetivo;
      Im->T_d = 0;
      m = anterior_no(m);
      OBJ_alterado = TRUE;
    }
    else {
      no n = anterior_no(m);
      remove_no(m, multi_intervalo(OBJ), destroi_intervalo);
      m = n;
      OBJ_alterado = TRUE;
    }

  if (!primeiro_no(multi_intervalo(OBJ)))
    enfileira(cria_intervalo(0, 0, 0, 0), multi_intervalo(OBJ));

  // se o processo anterior alterou a variável objetivo, é preciso aplicar a consistência novamente
  if (OBJ_alterado) {
    if (FULL_CONSISTENCY) {
      lista vars = cria_lista();
      enfileira(OBJ, vars);
      ramo_atual->conflito = global_consistency(C, vars);
    }
    else {
      ramo_atual->conflito = directional_consistency(C, NULL,  -1);
      if (!(ramo_atual->conflito))
        ramo_atual->conflito = directional_consistency(C, NULL, 1);
    }
    salva(C, ramo_atual);
  }
}

//--------------------------------------------------------------------------

// Essa função vai percorrer a pilha de restrições insatisfatíveis e escolher uma ramificação para continuar a busca.
// Para cada restrição insatisfatível ELEGÍVEL é testado um conjunto de 4 opções: cada cauda e cada lado, todas são empilhadas no backtracking.
// Para cada conjunto escolhe-se heuristicamente a melhor opção e armazena o seu nó do backtracking em n (mas apenas se não há conflito)
// Retorna o n da primeira ramificação encontrada não conflitante.
// A primeira heurística é respeitar a pilha, portanto, a primeira ramificação que der certo será retornada.
// A segunda heurística é definidA por "aplica_heuristica_erro_minimo_nos_arcos()".copel grupo b
// Parâmetro verifica_cabeca_degenerada: se for TRUE, a restrição é elegível apenas se não tiver a cabeça degenerada
no escolhe_variavel_e_ramifica(lista insatisfativeis, ramo ramo_atual, lista backtracking, int verifica_cabeca_degenerada, epifita C) {

  for (no m = primeiro_no(insatisfativeis); m; m = proximo_no(m)) {
    restricao R = objeto(m);

    int elegivel = !verifica_cabeca_degenerada || !M_intervalos_degenerados(multi_intervalo(cabeca(R)));

    if (elegivel){
      Variavel vars_ramif[2];
      vars_ramif[0] = cauda_E(R);
      vars_ramif[1] = cauda_D(R);

      char lados_ramif[2];
      lados_ramif[0] = 'b';
      lados_ramif[1] = 'a';

      double heuristica_min = INFINITY;
      double heuristica_melhor = INFINITY;
      no n = NULL;

      // há 4 casos de ramificação para a restrição R do conjunto Omega
      for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
          aplica(C, valoracao(ramo_atual));
          empilha(ramifica(vars_ramif[i], R, lados_ramif[j], ramo_atual, C), backtracking);
          if (!conflito(objeto(primeiro_no(backtracking))))
            aplica_heuristica_erro_minimo_nos_arcos(backtracking, erro_RAC(R), &heuristica_min, &heuristica_melhor, &n);
        }

      if (n) return n;
    }
  }
  return NULL;
}

//--------------------------------------------------------------------------

// para DEBUG apenas.
void imprime_pilha_de_backtracking(lista backtracking) {

  no m = primeiro_no(backtracking);
  printf("%s: ", nome(var_decidida(objeto(m))));
  imprime_Mintervalo(valor_decidido(objeto(m)));
  printf(" -> %d\n", nivel(objeto(m)));
  printf("========\n");

  for (m = primeiro_no(backtracking); m; m = proximo_no(m)) {
    printf("%s: ", nome(var_decidida(objeto(m))));
    imprime_Mintervalo(valor_decidido(objeto(m)));
    printf(" nivel = %d (%d) [%d]\n", nivel(objeto(m)), conflito(objeto(m)), jah_visitado(objeto(m)));
    printf(".");
  }
  printf("\n");
  printf("========\n");
}

//--------------------------------------------------------------------------

// InDE: o B&B serve para criar a populacao inicial. Assim, faz o B&B até atingir a MAX_PROFUNDIDADE,
// usando os subproblemas encontrados (e consistentes) para criar a populacao. Retorna uma lista de valorações.
lista branch_and_bound(epifita C, unsigned int MAX_PROFUNDIDADE, double *tempo_de_execucao) {

  // para calcular o tempo de execução
  struct timeval st, et;
  gettimeofday(&st , NULL);

  // declaração de variáveis auxiliares
  no n;
  restricao R;

  // OBJ vai ser sempre a variável objetivo da decomposição epífita
  Variavel OBJ = var_objetivo(C);

  // processamento inicial para o primeiro nó do backtracking
  int conflitante;
  if (FULL_CONSISTENCY)
    conflitante = global_consistency(C, NULL);
  else
    conflitante = directional_consistency(C, NULL, 1);

  if (conflitante) return NULL;

  // listas
  lista insatisfativeis;
  lista backtracking = cria_lista();
  lista populacao = cria_lista();

  ramo r = cria_ramo(OBJ, copia_Mintervalo(multi_intervalo(OBJ)), NULL, FALSE, C);
  empilha(r, backtracking);

  // Enquanto há possibilidades de busca, continua a execução.
  // o algoritmo só pode parar depois de percorrer todos os ramos ou podá-los
  // ou até que atinja o timeout.

  unsigned int timeouted = FALSE;
  while (primeiro_no(backtracking) && !timeouted) {

    // imprime_pilha_de_backtracking(backtracking);

    // verifica profundidade e aplica valoracao atual na epifita
    no no_atual = primeiro_no(backtracking);
    ramo ramo_atual = objeto(no_atual);
    if (nivel(ramo_atual) > (int)MAX_PROFUNDIDADE)
      ramo_atual->conflito = TRUE;
    aplica(C, valoracao(ramo_atual));

    // Poda o multi-intervalo objetivo com base no valor otimo encontrado até aqui
    // Obs: no InDE essa poda não deve ser feita
    //upper_bound(valor_objetivo, ramo_atual, C) {

    // se não houve conflito verificamos se eh solucao
    if (!conflito(ramo_atual)) {

      // no InDE, a busca local não deve ser aplicada (deseja-se apenas particionar os intervalos).
      insatisfativeis = cria_lista();
      // busca_local(C, insatisfativeis);

      // Aqui tem um problema assim: pode ser que em algum nó a busca local
      // não encontre solução e também não coloque ninguém da lista de insatisfativeis,
      // por exemplo, quando o intervalo objetivo é [-inf, inf] ou há erros numéricos
      // Nesse caso, vou adicionar todo mundo da lista.
      if (primeiro_no(insatisfativeis) == NULL)
        for (n = primeiro_no(arvores(C)); n; n = proximo_no(n)) {
          R = arco(objeto(n));
          if (R && !eh_RAC(R))
            empilha(R, insatisfativeis);
        }

      n = escolhe_variavel_e_ramifica(insatisfativeis, ramo_atual, backtracking, TRUE, C);

      // Se não encontrou variável para ramificar vai tentar de volta, mas
      // dessa vez incluindo cabeca(R) degeneradas
      if (!n)
        n = escolhe_variavel_e_ramifica(insatisfativeis, ramo_atual, backtracking, FALSE, C);

      if (!n)
        n = no_atual;

      // if (n == no_atual) printf("putz\n");

      ramo_atual->conflito = remove_todos_exceto(n, backtracking, no_atual);
      //retrocesso = aplica_heuristica_minimo_de_funcao(backtracking, no_atual);

      destroi_lista(insatisfativeis, nao_destroi);
    }

    // esse ramo já foi visitado
    ramo_atual->jah_visitado = TRUE;

    // se a valoração tem conflito, remove ela e todas as outras já visitadas do backtracking
    if (conflito(ramo_atual))
      while (primeiro_no(backtracking) && jah_visitado(objeto(primeiro_no(backtracking)))) {
        ramo r_tmp = objeto(primeiro_no(backtracking));
        // InDE: empilha na pilha da populacao as valorações do último nível do B&B
        if ((nivel(r_tmp) == (int)MAX_PROFUNDIDADE) && !conflito(r_tmp)) {
          empilha(r_tmp, populacao);
          remove_no(primeiro_no(backtracking), backtracking, nao_destroi);
        }
        else
          remove_no(primeiro_no(backtracking), backtracking, destroi_ramo);
      }

    // volta lá pra cima
    gettimeofday(&et , NULL);
    timeouted = (et.tv_sec - st.tv_sec > TIMEOUT);
  }

  destroi_lista(backtracking, nao_destroi);

  // DEBUG
  if (DEBUG) {
    printf("=> %llu valorações deram erro.\n", DEBUG_ERRO_INSTANCIACAO);
    printf("=> %llu consistências atingiram MAX_STEPS.\n", DEBUG_MAX_STEPS_REACHED);
  }

  // seta o tempo de execução
  if (timeouted)
    *tempo_de_execucao = -1;
  else {
    gettimeofday(&et , NULL);
    *tempo_de_execucao = (et.tv_sec - st.tv_sec) + ((double)(et.tv_usec - st.tv_usec))/1000000.0;
  }

  //return otimo_global;
  // No InDE o B&B devolve a populacao ao inves do otimo_global.
  return populacao;
}

//--------------------------------------------------------------------------
