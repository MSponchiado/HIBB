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
 *  Biblioteca intervalar.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __INTERVALO__
#define __INTERVALO__

#include "lista.h"
#include "real.h"
#include "../constantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern double DELTA;                    // granularidade minima
extern int ESCOLHA_ALEATORIA_INDE;      // se escolhe o valor do meio do intervalo (FALSE) ou aleatorio (TRUE)

typedef struct t_intervalo *intervalo;

// intervalo eh constituído dos limitantes e topologia (aberto ou fechado,
// à esquerda ou direita)
typedef struct t_intervalo {

  int T_e, T_d;
  double a, b;

} t_intervalo;

//--------------------------------------------------------------------------

intervalo cria_intervalo(int e, int d, double a, double b);

void destroi_intervalo(void *X);

intervalo copia_intervalo(intervalo X);

int Te(intervalo X);

int Td(intervalo X);

double inf(intervalo X);

double sup(intervalo X);

int eh_vazio(intervalo X);

double comprimento(intervalo X);

void imprime_intervalo(intervalo X);

// retorna o menor/maior valor de X
double i_min(intervalo X);

double i_max(intervalo X);

// retorna 1 se X está contido em Y
int i_subset(intervalo X, intervalo Y);

int degenerado(intervalo X);

int i_cap(intervalo X, intervalo Y);

// cria um novo intervalo com a união e o retorna
intervalo i_cup(intervalo X, intervalo Y);

void real_valuation(intervalo X, char pos);

void particiona(intervalo I, char lado, double delta);

// Considerando que M e N são listas de intervalos: M <- M U N,
// de forma que não tenha intervalos redundantes e seja ordenado.
// A lista N é destruida
void merge_ordenado(lista M, lista N);

// operações retornam uma lista de intervalos
//--------------------------------------------------------------------------
lista i_neutro(intervalo A, intervalo B);
//--------------------------------------------------------------------------
lista i_add(intervalo A, intervalo B);
lista i_add_inv(intervalo A, intervalo B);

lista i_sub(intervalo A, intervalo B);
lista i_sub_inv(intervalo A, intervalo B);
//--------------------------------------------------------------------------
lista i_mul(intervalo A, intervalo B);
lista i_mul_inv(intervalo A, intervalo B);

lista i_div(intervalo A, intervalo B);
lista i_div_inv(intervalo A, intervalo B);
//--------------------------------------------------------------------------
lista i_pow(intervalo A, intervalo B);
lista i_pow_inv(intervalo A, intervalo B);

lista i_sqr(intervalo A, intervalo B);
lista i_sqr_inv(intervalo A, intervalo B);

lista i_lg(intervalo A, intervalo B);
lista i_lg_inv(intervalo A, intervalo B);
//--------------------------------------------------------------------------
lista i_abs(intervalo A, intervalo B);
lista i_abs_inv(intervalo A, intervalo B);

lista i_iabs(intervalo A, intervalo B);
lista i_iabs_inv(intervalo A, intervalo B);

lista i_cabs(intervalo A, intervalo B);
lista i_cabs_inv(intervalo A, intervalo B);
//--------------------------------------------------------------------------
lista i_sgn(intervalo A, intervalo B);
lista i_sgn_inv(intervalo A, intervalo B);

lista i_val(intervalo A, intervalo B);
lista i_val_inv(intervalo A, intervalo B);

lista i_csgn(intervalo A, intervalo B);
lista i_csgn_inv(intervalo A, intervalo B);
//--------------------------------------------------------------------------
lista i_Max(intervalo A, intervalo B);
lista i_Max_inv(intervalo A, intervalo B);

lista i_iMax(intervalo A, intervalo B);
lista i_iMax_inv(intervalo A, intervalo B);

lista i_cMax(intervalo A, intervalo B);
lista i_cMax_inv(intervalo A, intervalo B);

lista i_Min(intervalo A, intervalo B);
lista i_Min_inv(intervalo A, intervalo B);

lista i_iMin(intervalo A, intervalo B);
lista i_iMin_inv(intervalo A, intervalo B);

lista i_cMin(intervalo A, intervalo B);
lista i_cMin_inv(intervalo A, intervalo B);
//--------------------------------------------------------------------------
lista i_exp(intervalo A, intervalo B);
lista i_exp_inv(intervalo A, intervalo B);

lista i_log(intervalo A, intervalo B);
lista i_log_inv(intervalo A, intervalo B);

lista i_cexp(intervalo A, intervalo B);
lista i_cexp_inv(intervalo A, intervalo B);
//--------------------------------------------------------------------------
intervalo f(intervalo A, double fa, double fb);
intervalo f_max(intervalo A, double fa, double fb);
intervalo f_min(intervalo A, double fa, double fb);
lista i_sincos(intervalo A, int sin);

lista i_sin(intervalo A, intervalo B);
lista i_sin_inv(intervalo A, intervalo B);

lista i_arcs(intervalo A, intervalo B);
lista i_arcs_inv(intervalo A, intervalo B);

lista i_csin(intervalo A, intervalo B);
lista i_csin_inv(intervalo A, intervalo B);

lista i_cos(intervalo A, intervalo B);
lista i_cos_inv(intervalo A, intervalo B);

lista i_arcc(intervalo A, intervalo B);
lista i_arcc_inv(intervalo A, intervalo B);

lista i_ccos(intervalo A, intervalo B);
lista i_ccos_inv(intervalo A, intervalo B);

lista i_tan(intervalo A, intervalo B);
lista i_tan_inv(intervalo A, intervalo B);

lista i_arct(intervalo A, intervalo B);
lista i_arct_inv(intervalo A, intervalo B);

lista i_ctan(intervalo A, intervalo B);
lista i_ctan_inv(intervalo A, intervalo B);
//--------------------------------------------------------------------------

#endif
