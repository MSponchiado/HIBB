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
 *  Biblioteca multi-intervalar.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __MINTERVALO__
#define __MINTERVALO__

#include "intervalo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Um multi-intervalo é uma lista de intervalos
typedef lista Mintervalo;

//--------------------------------------------------------------------------

Mintervalo cria_Mintervalo(void);

void destroi_Mintervalo(void *X);

Mintervalo copia_Mintervalo(Mintervalo X);

void imprime_Mintervalo(Mintervalo X);

int M_eh_vazio(Mintervalo X);

int M_degenerado(Mintervalo X);

double M_comprimento(Mintervalo X);

int M_intervalos_degenerados(Mintervalo X);

void limpa_Mintervalo(Mintervalo X);

// X <- X intersecao Y.
// devolve 1 em caso de sucesso e alteração de X
// e 0 em caso de falha ou de X == X intersecao Y
int Mcap(Mintervalo X, Mintervalo Y);

// opera X com Y.
// a operação também deve ser passada como parâmetro.
// pela definição X op Y é o resultado de op aplicado a todo
// par de intervalos de ambos X e Y
Mintervalo Mop(Mintervalo X, lista i_ope(intervalo A, intervalo B), Mintervalo Y);

// devolve 1 se X é subconjunto ou igual a Y
int Msubset(Mintervalo X, Mintervalo Y);

// particiona X (verificando sempre a granularidade_minima), ficando com o
// o lado esquerdo ('a') ou o direito ('b') desse multi-intervalo.
void M_particiona(Mintervalo X, char lado, double delta);

// atribui a X um valor double que pode ser min X, se pos == 'a', ou max X, se
// pos == 'c', ou um valor do meio de X, se pos == 'b'.
void M_real_valuation(Mintervalo X, char pos);
    
//--------------------------------------------------------------------------

#endif
