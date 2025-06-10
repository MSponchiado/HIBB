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
 *  Implementação de uma busca local simples.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __BUSCA_LOCAL__
#define __BUSCA_LOCAL__

#include "epifita.h"
#include <stdio.h>
#include <stdlib.h>

extern unsigned long long int DEBUG_ERRO_INSTANCIACAO;    // conta a quantidade de instanciações que apresentaram erros numéricos

//--------------------------------------------------------------------------
// Essa função vai considerar que N é um multi-intervalo de intervalos degenerados
// (ex: <[-2,-2], [2,2]>) e vai retornar qual o valor no multi-intervalo M mais
// próximo de N, isto é, mais próximo de qualquer um dos valores nos intervalos
// degenerados de N. A variável *distancia será setada com a distância entre esses
// valores.
double valor_mais_proximo_entre(Mintervalo M, Mintervalo N, double *distancia);

//--------------------------------------------------------------------------
// Devolve um valor do multi-intervalo M mais próximo do valor x e seta a
// variável *distancia com a distância entre esses valores. Logo, se x é um
// valor contido em M, o valor retornado será o próprio x, com distância 0.
double valor_mais_proximo_em(Mintervalo M, double x, double *distancia);

//--------------------------------------------------------------------------

// O problema é: instanciar uma variável X de uma restrição R dada que as outras variáveis
// já estão valoradas. Bastaria operar estas variáveis e o resultado ser atribuído a X
// (haveria um "erro" se a restrição não fosse RAC, que é a menor distância entre valores de X e o
// resultado da operação). Mas além disso, pode ser que 2 valores reais operados resultem em um
// intervalo (ex: 0/0 = (-inf,+inf)). Nesse caso, a variável X pode assumir qualquer valor do seu
// domínio, pois 0 = 0*X. O problema é que eu também quero saber qual é o "erro" dessa instanciação,
// isto é, a distância entre o valor que vou atribuir pra X e o resultado da operação das caudas. Para
// calcular essa distância vou calcular na verdade a distância que ocorre na instanciação de uma OUTRA
// rotação da restrição. No exemplo, o erro de 0/0 seria o erro da rotação 0 = 0*X, que é zero.
// Assim, essa função recebe como entrada R e X e retorna dois multi-intervalos M e N, onde M é o multi-intervalo
// da cabeça da restrição ROTACIONADA e N é o resultado da operação da restrição ROTACIONADA.
void calcula_melhor_rotacao(restricao R, Variavel X, Mintervalo *M, Mintervalo *N);

//--------------------------------------------------------------------------

// Essa função vai efetuar a instanciação da variável X da restrição R com
// base nas outras variáveis dessa restrição (considera que as outras já estão
// instanciadas). Chama a função "calcula_melhor_rotacao()" para obter os multi-intervalos
// M e N, onde M é o multi-intervalo da cabeça da "melhor" rotacao e N é o resultado da operação das caudas.
// Em seguida, obtém o valor de M mais próximo a N e com esse valor faz a instanciação.
// Mas aqui tem um detalhe: pode ser que a "melhor" rotação não seja com a variável X na cabeça,
// nesse caso a variável X será instanciada com a sua mediana.
// Note que independente da "distância" do valor_mais_proximo, a instanciação sempre ocorre (o tratamento deve
// ser feito na chamada da função).
double infere_instanciacao(restricao R, Variavel X);

//--------------------------------------------------------------------------
// Busca local do OGRe.
// Returna TRUE se há solução com a precisão pré-estabelecida.
// Vai empilhando em insatisfativeis os arcos que não são satisfeitas com a precisão dada.
// Aqui há 4 possibilidades:
// - o mínimo do intervalo objetivo é -inf: encerra a busca e retorna FALSE
// - alguma restrição de alguma árvore não foi satisfeita com a precisão dada (erros numéricos): encerra a busca e retorna FALSE
// - algum arco não foi satisfeito com a precisão dada: completa a busca, mas retorna FALSE e a lista de insatisfativeis preenchida
// - retorna TRUE: é uma solução satisfeita com a precisão dada (a lista de insatisfatíveis estará vazia)
int busca_local(epifita C, lista insatisfativeis);

//--------------------------------------------------------------------------

#endif
