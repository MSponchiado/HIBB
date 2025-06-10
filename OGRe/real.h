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
 *  Biblioteca de operações de ponto flutuantes em R estendido.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __REAL__
#define __REAL__

#include <math.h>
#include <float.h>

extern double MAX_REAL;       // antes de infinito
extern double MAX_REAL_DIV_2; // valor medio entre 0 e MAX_REAL

extern double EPSILON;       // EPSILON para comparação de números reais
extern double EPSILON_REL;   // EPSILON para comparação usando erro relativo
extern unsigned int ERR_RELATIVO_VAL_GRANDES;  // tipo da comparação para valores grandes

extern unsigned int CMP_NORM; // normaliza valores comparados se forem iguais (x "==" y => x = y)

#define TRUE 1
#define FALSE 0

//--------------------------------------------------------------------------

// compara ponto flutuante
// op pode ser "<", "<=", "==", ">=", ">" e "!="
int r_cmp(double x, char *op, double y, double *ref);

// verifica se o numero é infinitou ou -infinito
int eh_infinito(double x);

int eh_minfinito(double x);

// valor absoluto
double r_abs(double x);

// demais operações
double r_add(double x, double y);

double r_sub(double x, double y);

double r_mul(double x, double y);

double r_div(double x, double y);

double r_pow(double x, int y);

double r_sqr(double x, int y);

double r_log(double base, double x);

double r_exp(double base, double x);

double r_ln(double x);

double r_sen(double x);

double r_cos(double x);

double r_tan(double x);

//--------------------------------------------------------------------------

#endif
