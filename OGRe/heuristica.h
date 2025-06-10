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
 *  Implementação de heurísticas para escolha de variáveis.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __HEURISTICA__
#define __HEURISTICA__

#include "branch-bound.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------

// Essa função é chamada depois de vários ramos filhos do atual serem inseridos.
// O objetivo aqui é tirar todo mundo exceto aquele que encontrou o menor valor 
// para o intervalo objetivo.
// Nessa função, também verificamos o caso no qual TODOS os filhos são conflitantes,
// isto é, tem intervalos vazios. Nesse caso, o no atual passa a ter um conflito
// e essa função retorna 1.
int aplica_heuristica_minimo_de_funcao(lista backtracking, no atual);

// Essa função é chamada depois de um nó ser inserido no backtracking.
// O erro passado como parâmetro é o max_erro_de_valoracao(C) da valoração inserida.
// Esse erro é comparado como o min passado como parâmetro (também com o best) e,
// se o erro for menor, isto é, se essa valoração é a melhor que as testadas anteriores, 
// atualiza esses valores e retorna esse nó ou retorna NULL.
// Obs.: diferente da heurística acima, que é chamada uma única vez, essa função deve ser
// chamada para cada nó inserido. Portanto, a função remove_todos_exceto() também deve ser
// chamada ao final.
void aplica_heuristica_erro_minimo_nos_arcos(lista backtracking, double erro, double *min, double *best, no *n);


// remove todos os nós do backtracking que estão entre o primeiro nó e o atual,
// exceto o nó n, se ele não for conflitante. Senão, exclui também o nó n.
// retorna 0 caso manteve n e 1 caso excluiu todo mundo.
int remove_todos_exceto(no n, lista backtracking, no atual);

//--------------------------------------------------------------------------

#endif
