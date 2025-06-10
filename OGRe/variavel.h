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
 *  Implementação da estrutura de dados para variaveis contínuas.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __VARIAVEL__
#define __VARIAVEL__

#include "multi-intervalo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct t_Variavel *Variavel;

// Uma Variavel tem um nome e um multi-intervalo.
// Além disso, tem uma lista de (apontadores para) restrições, 
// que são as restrições que usam essa Variavel (assim
// como a lista de restarcos que são os arcos que usam essa Variavel).
// Obs: um arco é um restrição do conjunto \Omega
typedef struct t_Variavel {

  char *nome;
  int index;
  int flag_index;

  Mintervalo multi_intervalo;

  lista restricoes;
  lista restarcos;
  
} t_Variavel;

//--------------------------------------------------------------------------

Variavel cria_Variavel(char *n, intervalo I, int ind);

void destroi_Variavel(void *X);

Variavel copia_Variavel(Variavel X, int flag_tipo);

int indexV(Variavel X);

int flag_index(Variavel X);

char *nome(Variavel X);

lista restricoes(Variavel X);

lista restarcos(Variavel X);

Mintervalo multi_intervalo(Variavel X);

// retorna 1 se nome(X) == s
// s eh char *, mas foi colocado como void *
// para ser usada no busca da lista.
int compara_Variavel(void *X, void *s);

void imprime_Variavel(Variavel X);

//--------------------------------------------------------------------------

#endif
