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
 *  Implementação da estrutura de dados para árvores de restrições.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "arvore_restricoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


double ERRO_RAC = 0.1;                          // Default
unsigned int TIPO_ERRO_RAC = ERRO_ABS_REL;      // Default

//--------------------------------------------------------------------------

restricao cria_restricao(Variavel X, Variavel Y, Variavel Z, int operador) {

  restricao R = (restricao) malloc(sizeof(t_restricao));
	if (!R) {
		printf("ERRO: Impossível criar restrição\n");
		return NULL;
	}

  R->X = X;
  R->Y = Y;
  R->Z = Z;
  R->op = operador;
  R->pai = NULL;
  R->filhos = cria_lista();
  if (!R->filhos) {
    free(R);
    return NULL;
  }

  // atribui funcoes da restricao com base no operador
  if (op(R) == OP) {
    R->f_op = i_neutro;
    R->f_iop = i_neutro;
    R->f_cop = i_neutro;
  }
  else if (op(R) == IOP) {
	R->f_op = i_neutro;
	R->f_iop = i_neutro;
	R->f_cop = i_neutro;
  }
  else if (op(R) == COP) {
    R->f_op = i_neutro;
	R->f_iop = i_neutro;
	R->f_cop = i_neutro;
  }
  else if (op(R) == ADD) {
	R->f_op = i_add;
	R->f_iop = i_sub;
	R->f_cop = i_sub;
  }
  else if (op(R) == SUB) {
    R->f_op = i_sub;
    R->f_iop = i_add;
    R->f_cop = i_sub_inv;
  }
  else if (op(R) == MUL) {
    R->f_op = i_mul;
    R->f_iop = i_div;
    R->f_cop = i_div;
  }
  else if (op(R) == DIV) {
    R->f_op = i_div;
    R->f_iop = i_mul;
    R->f_cop = i_div_inv;
  }
  else if (op(R) == POW) {
    R->f_op = i_pow;
    R->f_iop = i_sqr;
    R->f_cop = i_lg;
  }
  else if (op(R) == SQR) {
    R->f_op = i_sqr;
    R->f_iop = i_pow;
    R->f_cop = i_lg_inv;
  }
  else if (op(R) == LG) {
    R->f_op = i_lg;
    R->f_iop = i_pow_inv;
    R->f_cop = i_sqr_inv;
  }
  else if (op(R) == SIN) {
    R->f_op = i_sin;
    R->f_iop = i_arcs;
    R->f_cop = i_csin;
  }
  else if (op(R) == ARCS) {
    R->f_op = i_arcs;
    R->f_iop = i_sin;
    R->f_cop = i_csin_inv;
  }
  else if (op(R) == CSIN) {
    R->f_op = i_csin;
    R->f_iop = i_sin_inv;
    R->f_cop = i_arcs_inv;
  }
  else if (op(R) == COS) {
    R->f_op = i_cos;
    R->f_iop = i_arcc;
    R->f_cop = i_ccos;
  }
  else if (op(R) == ARCC) {
    R->f_op = i_arcc;
    R->f_iop = i_cos;
    R->f_cop = i_ccos_inv;
  }
  else if (op(R) == CCOS) {
    R->f_op = i_ccos;
    R->f_iop = i_cos_inv;
    R->f_cop = i_arcc_inv;
  }
  else if (op(R) == TAN) {
    R->f_op = i_tan;
    R->f_iop = i_arct;
    R->f_cop = i_ctan;
  }
  else if (op(R) == ARCT) {
    R->f_op = i_arct;
    R->f_iop = i_tan;
    R->f_cop = i_ctan_inv;
  }
  else if (op(R) == CTAN) {
    R->f_op = i_ctan;
    R->f_iop = i_tan_inv;
    R->f_cop = i_arct_inv;
  }
  else if (op(R) == EXP) {
    R->f_op = i_exp;
    R->f_iop = i_log;
    R->f_cop = i_cexp;
  }
  else if (op(R) == LOG) {
    R->f_op = i_log;
    R->f_iop = i_exp;
    R->f_cop = i_cexp_inv;
  }
  else if (op(R) == CEXP) {
    R->f_op = i_cexp;
    R->f_iop = i_exp_inv;
    R->f_cop = i_log_inv;
  }
  else if (op(R) == ABS) {
    R->f_op = i_abs;
    R->f_iop = i_iabs;
    R->f_cop = i_cabs;
  }
  else if (op(R) == IABS) {
    R->f_op = i_iabs;
    R->f_iop = i_abs;
    R->f_cop = i_cabs_inv;
  }
  else if (op(R) == CABS) {
    R->f_op = i_cabs;
    R->f_iop = i_abs_inv;
    R->f_cop = i_iabs_inv;
  }
  else if (op(R) == SGN) {
    R->f_op = i_sgn;
    R->f_iop = i_val;
    R->f_cop = i_csgn;
  }
  else if (op(R) == VAL) {
    R->f_op = i_val;
    R->f_iop = i_sgn;
    R->f_cop = i_csgn_inv;
  }
  else if (op(R) == CSGN) {
    R->f_op = i_csgn;
    R->f_iop = i_sgn_inv;
    R->f_cop = i_val_inv;
  }
  else if (op(R) == MAX) {
    R->f_op = i_Max;
    R->f_iop = i_iMax;
    R->f_cop = i_cMax;
  }
  else if (op(R) == IMAX) {
    R->f_op = i_iMax;
    R->f_iop = i_Max;
    R->f_cop = i_cMax_inv;
  }
  else if (op(R) == CMAX) {
    R->f_op = i_cMax;
    R->f_iop = i_Max_inv;
    R->f_cop = i_iMax_inv;
  }
  else if (op(R) == MIN) {
    R->f_op = i_Min;
    R->f_iop = i_iMin;
    R->f_cop = i_cMin;
  }
  else if (op(R) == IMIN) {
    R->f_op = i_iMin;
    R->f_iop = i_Min;
    R->f_cop = i_cMin_inv;
  }
  else if (op(R) == CMIN) {
    R->f_op = i_cMin;
    R->f_iop = i_Min_inv;
    R->f_cop = i_iMin_inv;
  }
  else {
    R->f_op = i_neutro;
    R->f_iop = i_neutro;
    R->f_cop = i_neutro;
  }

  return R;
}

//--------------------------------------------------------------------------

void destroi_restricao(void *R) {

  destroi_lista(filhos(R), nao_destroi);
  if (cabeca(R)) destroi_Variavel(cabeca(R));
  if (cauda_E(R)) destroi_Variavel(cauda_E(R));
  if (cauda_D(R)) destroi_Variavel(cauda_D(R));

  free(R);
}

//--------------------------------------------------------------------------

restricao pai(restricao R) { return R->pai; }

lista filhos(restricao R) { return R->filhos; }

Variavel cabeca(restricao R) { return R->X; }

Variavel cauda_E(restricao R) { return R->Y; }

Variavel cauda_D(restricao R) { return R->Z; }

int op(restricao R) { return R->op; }

//--------------------------------------------------------------------------

// verifica se X é a cabeça da restrição R
int compara_cabeca_restricao(void *X, void *R) {
  return  (X == cabeca(R));
}

// verifica se as restrições R e S são iguais
int compara_restricao(void *R, void *S) {
  return  (R == S);
}

//--------------------------------------------------------------------------

// retorna 1 se R é RAC com tolerância ERRO_RAC
int eh_RAC(restricao R) {
  return r_cmp(erro_RAC(R), "<=", ERRO_RAC, NULL);
}

//--------------------------------------------------------------------------

arvore cria_arvore(void) {

  arvore T = (arvore) malloc(sizeof(t_arvore));
  if (T) {
    T->raiz = NULL;
    T->arco = NULL;
    T->variaveis = cria_lista();
    T->odv = cria_lista();
    if (!T->variaveis || !T->odv) {
      free(T);
      return NULL;
    }
  }
  else
    printf("ERRO: Impossível criar árvore\n");

  return T;
}

//--------------------------------------------------------------------------

void destroi_arvore(void *T) {

  destroi_lista(variaveis(T), destroi_Variavel);
  destroi_lista(odv(T), nao_destroi);

  free(T);
}

//--------------------------------------------------------------------------

restricao raiz(arvore T) { return T->raiz; }

restricao arco(arvore T) { return T->arco; }

lista variaveis(arvore T) { return T->variaveis; }

lista odv(arvore T) { return T->odv; }

//--------------------------------------------------------------------------

arvore adiciona_arco(restricao R, arvore T) {

  T->arco = R;

  enfileira(R, restarcos(cabeca(R)));
  enfileira(R, restarcos(cauda_E(R)));
  enfileira(R, restarcos(cauda_D(R)));

  return T;
}

//--------------------------------------------------------------------------

arvore adiciona_restricao(restricao R, arvore T, no p) {

  // se a cabeça de R não está em T, então R será a raiz
  if (!p) {
    T->raiz = R;
    enfileira(cabeca(R), variaveis(T));
  }

  // senão, verifico se a raiz da arvore é restrição fantasma. Se for, troca por R
  // Obs: garantidamente, se for fantasma, é porque era a única restrição da árvore
  else if (T->raiz->Y->nome[0] == '#') {

    // assert:
    if (proximo_no(primeiro_no(restricoes(T->raiz->X))) || 
        proximo_no(primeiro_no(restricoes(T->raiz->Y))) ||
        proximo_no(primeiro_no(restricoes(T->raiz->Z)))) {
        printf("ERROR: verificar assert do arquivo arvore_restricoes.c, linha 359\n");
        exit(1);
    }
    destroi_lista(restricoes(T->raiz->X), nao_destroi);
    destroi_lista(restricoes(T->raiz->Y), nao_destroi);
    destroi_lista(restricoes(T->raiz->Z), nao_destroi);
    T->raiz->X->restricoes = cria_lista();
    T->raiz->Y->restricoes = cria_lista();
    T->raiz->Z->restricoes = cria_lista();

    T->raiz = R;

    destroi_lista(variaveis(T), nao_destroi);
    T->variaveis = cria_lista();
    enfileira(cabeca(R), variaveis(T));
  }
  // senão, R será filho da primeira restrição que contém a cabeça de R
  // (seja como cauda ou também cabeça)
  else {
    restricao aux = objeto(primeiro_no(restricoes(objeto(p))));
    enfileira(R, filhos(aux));
    R->pai = aux;
  }

  // em qualquer caso, atualiza todas as listas da árvore
  enfileira(cauda_E(R), variaveis(T));
  enfileira(cauda_D(R), variaveis(T));

  enfileira(R, restricoes(cabeca(R)));
  enfileira(R, restricoes(cauda_E(R)));
  enfileira(R, restricoes(cauda_D(R)));

  return T;
}

//--------------------------------------------------------------------------

void imprime_restricoes(restricao R, int recursivamente) {

  imprime_Variavel(cabeca(R));
  printf(" = ");
  imprime_Variavel(cauda_E(R));
  printf(" %s ", opcode2str(op(R)));
  imprime_Variavel(cauda_D(R));
  printf("\n");

  if (recursivamente) {
    no p;
    for (p = primeiro_no(filhos(R)); p; p = proximo_no(p)) {
      printf("%s = %s %s %s, ", nome(cabeca(objeto(p))), nome(cauda_E(objeto(p))), opcode2str(op(objeto(p))), nome(cauda_D(objeto(p))));
    }
    printf("\n");
    for (p = primeiro_no(filhos(R)); p; p = proximo_no(p))
      imprime_restricoes(objeto(p), 1);
  }
}

//--------------------------------------------------------------------------

// consistencia de arco na arvore T, linear no numero de restrições.
// retorna 1 se alterou algum ou 0 caso a árvore já fosse consistente
int arc_consistency(arvore T, int *conflito) {

  int alterei = DAC(ultimo_no(odv(T)), conflito);

  if (*conflito) return TRUE;

  if (DAC_reverse(primeiro_no(odv(T)), conflito))
    alterei = TRUE;

  return alterei;
}

//--------------------------------------------------------------------------

// Directional Arc Consistency, ordenado das folhas para a raiz
// Obs.: pode ser usado tanto para uma unica arvore quanto para um conjunto,
// isto eh, toda a Árvore Epífita
// Retorna TRUE se algum intervalo foi alterado
// Seta a variável conflito para TRUE se algum intervalo tornou-se vazio
int DAC(no inicial, int *conflito) {

  int alterei = FALSE;
  no p;

  // Aplica a consistência local na ordem selecionada,
  // verificando se a consistência alterou algum valor ou não
  for (p = inicial; p; p = anterior_no(p)) {
    if (local_consistency(objeto(p), conflito))
      alterei = TRUE;
    if (*conflito) return TRUE;
  }

  return alterei;
}

//--------------------------------------------------------------------------

// Directional Arc Consistency, ordenado da raiz para as folhas
// Obs.: pode ser usado tanto para uma unica arvore quanto para um conjunto,
// isto eh, toda a Árvore Epífita
// Retorna TRUE se algum intervalo foi alterado
// Seta a variável conflito para TRUE se algum intervalo tornou-se vazio
int DAC_reverse(no inicial, int *conflito) {

  int alterei = FALSE;
  no p;

  // Aplica a consistência local na ordem selecionada,
  // verificando se a consistência alterou algum valor ou não
  for (p = inicial; p; p = proximo_no(p)) {
    if (local_consistency(objeto(p), conflito))
      alterei = TRUE;
    if (*conflito) return TRUE;
  }

  return alterei;
}

//--------------------------------------------------------------------------

// aplica arc consistency na restrição R
// retorna um inteiro de 3 digitos que representa um número binário cujos
// dígitos significam: d1 d2 d3 --> d1 é se alterou a cauda_D(R)
// d2 é se alterou a cauda_E(R) e d3 é se alterou a cabeca(R).
int local_consistency(restricao R, int *conflito) {

  int alterei = 0;

  Mintervalo lX, lY, lZ;

  *conflito = FALSE;

  // imprime_restricoes(R, 0);
  // printf("\n");

  lX = Mop(multi_intervalo(R->Y), R->f_op, multi_intervalo(R->Z));
  if (Mcap(multi_intervalo(R->X), lX)) alterei = 1;
  destroi_Mintervalo(lX);

  lY = Mop(multi_intervalo(R->X), R->f_iop, multi_intervalo(R->Z));
  if (Mcap(multi_intervalo(R->Y), lY)) alterei = alterei + 10;
  destroi_Mintervalo(lY);

  lZ = Mop(multi_intervalo(R->X), R->f_cop, multi_intervalo(R->Y));
  if (Mcap(multi_intervalo(R->Z), lZ)) alterei = alterei + 100;
  destroi_Mintervalo(lZ);

  if (M_eh_vazio(multi_intervalo(R->X)) ||
      M_eh_vazio(multi_intervalo(R->Y)) ||
      M_eh_vazio(multi_intervalo(R->Z))) {

    *conflito = TRUE;
  }

  // imprime_restricoes(R, 0);
  // printf("\n");

  return alterei;
}

//--------------------------------------------------------------------------
// Daqui para baixo são funções para calcular qual é o erro RAC da restrição
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------

// transforma o erro absoluto em relativo, se for o caso
double normaliza_erro_RAC(double EA, double ref) {

  if (TIPO_ERRO_RAC == ERRO_ABS_REL) {
    double ER = r_abs(r_div(EA, ref));
    if (eh_infinito(EA) && (eh_minfinito(ref) || eh_infinito(ref)))
      ER = INFINITY;
    if (r_cmp(ER, "<", EA, &ER))
      return ER;
    else
      return EA;
  }

  if (TIPO_ERRO_RAC == ERRO_RELATIVO && r_cmp(ref, "!=", 0, &ref)) {
    double ER = r_abs(r_div(EA, ref));
    if (eh_infinito(EA) && (eh_minfinito(ref) || eh_infinito(ref)))
      ER = INFINITY;
    if (isnan(ER))
       printf("%.15f e %.15f\n", EA, ref);
    return ER;
  }

  return EA;
}
//--------------------------------------------------------------------------

// Função auxiliar da erro_de_valoracao(R):
// Recebe 2 multi-intervalos M e tmp, M \subseteq tmp, e retorna a maior
// distância entre dois pontos quaisquer tomados destes multi-intervalos.
// Essa distância pode ser absoluta ou relativa, dependendo do tipo de erro
// utilizado para calcular RAC.
double maior_distancia(Mintervalo M, Mintervalo tmp) {

  double ref = i_min(objeto(primeiro_no(tmp)));
  double dist = r_abs(r_sub(i_min(objeto(primeiro_no(M))), ref));
  double erro = normaliza_erro_RAC(dist, ref);

  //printf("%.15f %.15f %.15f\n", ref, dist, erro);

  no q = primeiro_no(tmp);

  for (no p = primeiro_no(M); p; p = proximo_no(p)) {
    if (proximo_no(p)) {
      ref = r_div(r_add(i_max(objeto(p)), i_min(objeto(proximo_no(p)))), 2);
      dist = r_abs(r_div(r_sub(i_max(objeto(p)), i_min(objeto(proximo_no(p)))), 2));

      while (r_cmp(i_max(objeto(q)), "<", ref, NULL))
        q = proximo_no(q);

      if (r_cmp(i_min(objeto(q)), "<", ref, NULL)) {
        dist = normaliza_erro_RAC(dist, ref);
        if (r_cmp(dist, ">", erro, &dist))
          erro = dist;
      }
      else {
        double val_d = r_sub(dist, r_abs(r_sub(ref, i_min(objeto(q)))));
        val_d = normaliza_erro_RAC(val_d, i_min(objeto(q)));

        double val_e = r_sub(dist, r_abs(r_sub(ref, i_max(objeto(anterior_no(q))))));
        val_e = normaliza_erro_RAC(val_e, i_max(objeto(anterior_no(q))));

        if (r_cmp(val_d, ">", erro, &val_d))
          erro = val_d;
        if (r_cmp(val_e, ">", erro, &val_e))
          erro = val_e;
      }
    }
    else {
      ref = i_max(objeto(ultimo_no(tmp)));
      dist = r_abs(r_sub(i_max(objeto(ultimo_no(M))), ref));
      dist = normaliza_erro_RAC(dist, ref);

      if (r_cmp(dist, ">", erro, &dist))
        erro = dist;
    }
  }
  return erro;
}
//--------------------------------------------------------------------------

// devolve o erro maximo que pode ocorrer na restricao R considerando uma instanciação
// das caudas para a cabeça.
double erro_RAC(restricao R) {

  Mintervalo tmp, M;
  double erro;

  // Aqui considero que M está contido em tmp, pq é consistente
  tmp = Mop(multi_intervalo(R->Y), R->f_op, multi_intervalo(R->Z));
  M = multi_intervalo(R->X);

  erro = maior_distancia(M, tmp);
  destroi_Mintervalo(tmp);

  return erro;
}
