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

// ver arquivo em https://sharelatex.c3sl.ufpr.br/project/5dfa73be7c2744007b120763
// ATENCAO: na modelagem apenas algumas dessas operacoes sao permitidas. Sao elas:
// OP, ADD, SUB, MUL, DIV, POW, SQR, SIN, ARCS, COS, ARCC, TAN, ARCT, EXP, LOG, ABS, SGN, MAX, MIN

#ifndef __OPERACOES__
#define __OPERACOES__

#define OP 1
#define IOP 2
#define COP 3

#define ADD 4       //  X = Y + Z
#define SUB 5       //  Y = X - Z

#define MUL 6       //  X = Y * Z
#define DIV 7       //  Y = X / z

#define POW 8 		//	X = Y ^ c                           c inteiro
#define SQR 9 		//	Y = X v c = sqr[c](X)				c inteiro
#define LG 10 		// 	c = X ~ Y = log_Y(X) = [-oo,+oo]	nao usar na modelagem

#define SIN 11		//	X = Y sin 1 = sin(Y)
#define ARCS 12		//	Y = X arcs 1 = arcsin(X)
#define CSIN 13		//	1 = X csin Y = [-oo,+oo]

#define COS 14		//	X = Y cos 1 = cos(Y)
#define ARCC 15		//	Y = X arcc 1 = arccos(X)
#define CCOS 16		//	1 = X ccos Y = [-oo,+oo]

#define TAN 17		//	X = Y tan 1 = tan(Y)
#define ARCT 18		//	Y = X arct 1 = arctan(X)
#define CTAN 19		//	1 = X ctan Y = [-oo,+oo]

#define EXP 20 		//	X = Y exp b = b^Y					0 < b != 1
#define LOG 21 		//	Y = X log b = log_b(X)				0 < b != 1
#define CEXP 22		//	b = X cexp Y = [-oo,+oo]

#define ABS 23		//	X = Y abs 1 = |Y|
#define IABS 24		//	Y = X iabs 1 = {r | |r| in X}
#define CABS 25		//	1 = X cabs Y = [-oo,+oo]

#define SGN 26		//	X = Y sgn 1 = sgn(Y)
#define VAL 27		//	Y = X val 1 = val(X) = {r | sgn(r) in X}
#define CSGN 28		//	1 = X csgn Y = [-oo,+oo]

#define MAX 29		//	X = Y max Z = {max{y,z} | y in Y and z in Z}
#define IMAX 30		//	Y = X imax Z = [-oo,+oo]
#define CMAX 31		//	Z = X cmax Y = [-oo,+oo]

#define MIN 32		//	X = Y min Z = {min{y,z} | y in Y and z in Z}
#define IMIN 33		//	Y = X imin Z = [-oo,+oo]
#define CMIN 34		//	Z	= X cmin Y = [-oo,+oo]

int opcode(char *operacao);
char *opcode2str(int op);

#endif
