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

#include "real.h"
#include <math.h>
#include <float.h>
#include <stdio.h>


double MAX_REAL = 999999999999999.0;    // os números representados abaixo deste diferem em menos de 1.0
double MAX_REAL_DIV_2 = 999999999.0;    // tentando considerar a distribuição da representação numérica

double EPSILON = DBL_EPSILON;
double EPSILON_REL = DBL_EPSILON;
unsigned int ERR_RELATIVO_VAL_GRANDES = TRUE;

unsigned int CMP_NORM = TRUE;

//--------------------------------------------------------------------------

/*
 Para mais referências sobre a comparação de ponto flutuantes:
 http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 */
int r_cmp(double x, char *op, double y, double *ref) {

  if (op[0] == '<' && op[1] == '=')
    return (r_cmp(x, "<", y, ref) || r_cmp(x, "==", y, ref));

  if (op[0] == '>' && op[1] == '=')
    return (r_cmp(x, ">", y, ref) || r_cmp(x, "==", y, ref));

  // casos infinitos:

  if (op[0] == '<') {
    if (eh_minfinito(x))
      return eh_minfinito(y) ? FALSE : TRUE;
    else if (eh_infinito(x))
      return FALSE;
    else if (eh_minfinito(y))
      return FALSE;
    else if (eh_infinito(y))
      return eh_infinito(x) ? FALSE : TRUE;
  }
  else if (op[0] == '>') {
    if (eh_minfinito(x))
      return FALSE;
    else if (eh_infinito(x))
      return eh_infinito(y) ? FALSE : TRUE;
    else if (eh_minfinito(y))
      return eh_minfinito(x) ? FALSE : TRUE;
    else if (eh_infinito(y))
      return FALSE;
  }
  else if (op[0] == '=') {
    if (eh_minfinito(x))
      return eh_minfinito(y) ? TRUE : FALSE;
    else if (eh_infinito(x))
      return eh_infinito(y) ? TRUE : FALSE;
    else if (eh_minfinito(y))
      return eh_minfinito(x) ? TRUE : FALSE;
    else if (eh_infinito(y))
      return eh_infinito(x) ? TRUE : FALSE;
  }
  else {
    if (eh_minfinito(x))
      return eh_minfinito(y) ? FALSE : TRUE;
    else if (eh_infinito(x))
      return eh_infinito(y) ? FALSE : TRUE;
    else if (eh_minfinito(y))
      return eh_minfinito(x) ? FALSE : TRUE;
    else if (eh_infinito(y))
      return eh_infinito(x) ? FALSE : TRUE;
  }

  // casos normais

  int iguais = FALSE;

  if ((eh_infinito(x) && eh_infinito(y)) || (eh_minfinito(x) && eh_minfinito(y))) {
    iguais = TRUE;
  }
  else {
    double diff = r_abs(r_sub(x, y));  // primeiro testa erro absoluto
    if (diff <= EPSILON)
      iguais = TRUE;
    else if (ERR_RELATIVO_VAL_GRANDES){
      double A = r_abs(x);        // depois erro relativo, se a flag estiver acionada
      double B = r_abs(y);
      double maior = (A > B) ? A : B;
      if (diff <= r_mul(maior, EPSILON_REL))
        iguais = TRUE;
    }
  }

  if (CMP_NORM && iguais && ref)
    *ref = y;

  if (op[0] == '<')
    return (!iguais && x < y);

  else if (op[0] == '>')
    return (!iguais && x > y);

  else if (op[0] == '=')
    return iguais;

  return !iguais;
}

//--------------------------------------------------------------------------

int eh_infinito(double x) {
  return (isinf(x) && x > 0);
}

//--------------------------------------------------------------------------

int eh_minfinito(double x) {
  return (isinf(x) && x < 0);
}

//--------------------------------------------------------------------------

double r_abs(double x) {
  return fabs(x);
}

//--------------------------------------------------------------------------

double r_add(double x, double y) {

  if ((eh_minfinito(x) && eh_infinito(y)) ||
      (eh_infinito(x) && eh_minfinito(y)))
    return 0;

  return x+y;
}

//--------------------------------------------------------------------------

double r_sub(double x, double y) {

  if ((eh_minfinito(x) && eh_minfinito(y)) ||
      (eh_infinito(x) && eh_infinito(y)))
    return 0;

  return x-y;
}

//--------------------------------------------------------------------------

double r_mul(double x, double y) {

  if ((eh_infinito(x) || eh_minfinito(x)) && r_cmp(y, "==", 0, &y))
    return 0;

  if ((eh_infinito(y) || eh_minfinito(y)) && r_cmp(x, "==", 0, &x))
    return 0;

  return x*y;
}

//--------------------------------------------------------------------------

double r_div(double x, double y) {

  // inf/inf e 0/0 são definidos como 0.
  // ATENCAO: isso dá erro no calculo de ERRO relativo para EA = inf e x = inf,
  // pois ER = EA/x deve ser inf ao invés de 0
  if (r_cmp(x, "==", 0, &x) || eh_minfinito(y) || eh_infinito(y))
    return 0;

  return x/y;
}

//--------------------------------------------------------------------------

double r_pow(double x, int y) {
  return pow(x, y);
}

//--------------------------------------------------------------------------

double r_sqr(double x, int y) {

  if (r_cmp(x, "<", 0, &x) && y % 2)
    return -pow(-x, (1.0/y));

  return pow(x, (1.0/y));
}

//--------------------------------------------------------------------------

double r_exp(double base, double x) {
	if (base > 1 && eh_infinito(x))
			return INFINITY;
	if (base > 1 && eh_minfinito(x))
			return 0;
	if (base < 1 && eh_infinito(x))
			return 0;
	if (base < 1 && eh_minfinito(x))
			return INFINITY;
	return pow(base, x);
}

//--------------------------------------------------------------------------

double r_log(double base, double x) {
	if (base > 1 && eh_infinito(x))
			return INFINITY;
	if (base > 1 && r_cmp(x, "==", 0, &x))
			return -INFINITY;
	if (base < 1 && eh_infinito(x))
			return -INFINITY;
	if (base < 1 && r_cmp(x, "==", 0, &x))
			return INFINITY;
	return log(x)/log(base);
}


//--------------------------------------------------------------------------

double r_sen(double x) {
	if (eh_infinito(x) || eh_minfinito(x)) // TODO: acho que isso nunca vai acontecer
		return 0;
	return sin(x);
}

//--------------------------------------------------------------------------

double r_cos(double x) {
	if (eh_infinito(x) || eh_minfinito(x)) // TODO: acho que isso nunca vai acontecer
		return 0;
	return cos(x);
}

//--------------------------------------------------------------------------

double r_tan(double x) {
	if (eh_infinito(x) || eh_minfinito(x)) // TODO: acho que isso nunca vai acontecer
		return 0;
	return tan(x);
}


