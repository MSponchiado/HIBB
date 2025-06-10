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
 *  Biblioteca para o pré-processador que rotaciona a Árvore Epífita.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "rotacionador_lib.h"
#include "operacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------
// Função que destroi um PointerRest

void destroi_PointerRest(void *Pr) {
  free(Pr);
}

//--------------------------------------------------------------------------
// Função que compara o nome de uma variável com
// uma string, utilizada na busca de variáveis.

char *nome(Var v) { return v->nome; }

int compara_Var(void *v, void *s) {
  return !strcmp(s, nome(v));
}

//--------------------------------------------------------------------------
// Função que lê variáveis do arquivo dado

char *le_variaveis(char *arq, lista variaveis, FILE *f) {

  int e, d;
  int erro = 0;
  double a, b;

  char s[1024];

  FILE *fv = fopen(arq, "r");

  while (fscanf(fv, "%s", s) != EOF) {

    Var v = (Var) malloc(sizeof(t_Var));
    v->nome = (char *) malloc(sizeof(char)*(strlen(s)+2));
    v->nivel = 0;
    v->cabeca_de_arco = 0;
    v->rest = cria_lista();

    // verifico se variável é raiz
    if (s[0] == 'f') {
      char *raiz = (char *) malloc(sizeof(char)*(strlen(s)+2));
      strcpy(raiz, &(s[1]));
      strcpy(v->nome, &(s[1]));
      fclose(fv);

      return raiz;
    }
    strcpy(v->nome, s);

    // leio demais informações e imprimo no arquivo

    // nome
    if (!fprintf(f, "v %s ", s)) erro = 1;

    // Te
    if (!fscanf(fv, "%d", &e)) erro = 1;
    if (!fprintf(f, "%c ", e ? '[' : '(')) erro = 1;

    // inf
    if (!fscanf(fv, "%lf", &a)) erro = 1;
    if (!fprintf(f, "%lf ", a)) erro = 1;

    // ,
    if (!fscanf(fv, "%s", s)) erro = 1;
    if (!fprintf(f, "%s ", s)) erro = 1;

    // sup
    if (!fscanf(fv, "%lf", &b)) erro = 1;
    if (!fprintf(f, "%lf ", b)) erro = 1;

    // Td
    if (!fscanf(fv, "%d", &d)) erro = 1;
    if (!fprintf(f, "%c\n", d ? ']' : ')')) erro = 1;

    if (erro) {
      printf("Erro na leitura das variaveis.\n");
      exit(1);
    }

    v->I = cria_intervalo(e, d, a, b);

    // salvo ela na lista de variaveis
    enfileira(v, variaveis);
  }

  fclose(fv);

  return NULL;
}

//--------------------------------------------------------------------------
// Função que lê restrições do arquivo dado

void le_restricoes(char *arq, lista variaveis, lista restricoes) {

  char s[1024];

  FILE *fr = fopen(arq, "r");

  while (fscanf(fr, "%s", s) != EOF) {
    int erro = 0;

    // x
    Rest R = (Rest) malloc(sizeof(t_Rest));
    no n = busca(s, variaveis, compara_Var);
    Var v = objeto(n);

    R->X = v;
    enfileira(R, v->rest);

    if (!fscanf(fr, "%s", s)) erro = 1; // =
    if (!fscanf(fr, "%s", s)) erro = 1; // y

    n = busca(s, variaveis, compara_Var);
    v = objeto(n);

    R->Y = v;
    enfileira(R, v->rest);

    if (!fscanf(fr, "%s", s)) erro = 1; // op
    R->op = opcode(s);

    if (!fscanf(fr, "%s", s)) erro = 1; // z

    if (erro) {
      printf("Erro na leitura das restricoes.\n");
      exit(1);
    }

    n = busca(s, variaveis, compara_Var);
    v = objeto(n);

    R->Z = v;
    enfileira(R, v->rest);

    R->cap = R->ok = 0;

    enfileira(R, restricoes);
  }

  fclose(fr);
}

//--------------------------------------------------------------------------
// Função que aplica várias podas num dado ramo

int poda(Rest R, int arcos_insatisfativeis, int cabecas_nao_auxiliares, int otimo_cabecas_nao_auxiliares) {

  // backtracking padrão
  if (R->cap == 3 && !(R->ok))
    return 1;

  // backtracking de constantes nos arcos
  if (R->cap == 2 && !PERMITE_CONS_ARCOS && (  (R->X->nivel == 0 && R->X->nome[0] == '@')
                                             ||(R->Y->nivel == 0 && R->Y->nome[0] == '@')
                                             ||(R->Z->nivel == 0 && R->Z->nome[0] == '@')
                                             ))
    return 1;

  // backtracking de igualdade nos arcos
  if (R->cap == 2 && arcos_insatisfativeis == MAX_ARCOS_UNSAT && ((R->X->nivel == 0 && !igualdade_no_arco(R, 0))
                                                                ||(R->Y->nivel == 0 && !igualdade_no_arco(R, 1))
                                                                ||(R->Z->nivel == 0 && !igualdade_no_arco(R, 2))
                                                                  ))
    return 1;

  // backtracking de cabeças não sendo vars auxiliares
  // if (R->cap == 2 && cabecas_nao_auxiliares == MAX_CABECAS_NAO_AUXILIARES && (  (R->X->nivel == 0 && R->X->nome[0] != 'z')
  //                                                                            ||(R->Y->nivel == 0 && R->Y->nome[0] != 'z')
  //                                                                            ||(R->Z->nivel == 0 && R->Z->nome[0] != 'z')
  //                                                                            ))
  //  return 1;

  // outro deste mas usando um otimo passado por parametro
  if (R->cap == 2 && cabecas_nao_auxiliares == otimo_cabecas_nao_auxiliares && (  (R->X->nivel == 0 && R->X->nome[0] != 'z')
                                                                                ||(R->Y->nivel == 0 && R->Y->nome[0] != 'z')
                                                                                ||(R->Z->nivel == 0 && R->Z->nome[0] != 'z')
                                                                                ))
    return 1;

  return 0;
}

//--------------------------------------------------------------------------
// Função que atualiza o conjunto cap dependendo do char no parâmetro

void atualiza_conjunto_cap(lista r, char s) {
  no n;

  for (n = primeiro_no(r); n; n = proximo_no(n)) {
    Rest R = objeto(n);
    if (s == '1')
      R->cap = 1;
    else if (s == '+')
      R->cap++;
    else if (s == '-') {
      R->cap--;
      R->ok = 0;
    }
  }
}

//--------------------------------------------------------------------------

//TODO: verificar essa função
lista opera(intervalo A, intervalo B, int op, int rot) {

  lista Resultado;

  if (op == OP) {
    if (rot == 0)
      Resultado = i_neutro(A, B);
    else if (rot == 1)
      Resultado = i_neutro(A, B);
    else
      Resultado = i_neutro(A, B);
  }
  else if (op == ADD) {
    if (rot == 0)
      Resultado = i_add(A, B);
    else if (rot == 1)
      Resultado = i_sub(A, B);
    else
      Resultado = i_sub(A, B);
  }
  else if (op == SUB) {
    if (rot == 0)
      Resultado = i_sub(A, B);
    else if (rot == 1)
      Resultado = i_add(A, B);
    else
      Resultado = i_sub(B, A);
  }
  else if (op == MUL) {
    if (rot == 0)
      Resultado = i_mul(A, B);
    else if (rot == 1)
      Resultado = i_div(A, B);
    else
      Resultado = i_div(A, B);
  }
  else if (op == DIV) {
    if (rot == 0)
      Resultado = i_div(A, B);
    else if (rot == 1)
      Resultado = i_mul(A, B);
    else
      Resultado = i_div(B, A);
  }
  else if (op == POW) {
    if (rot == 0)
      Resultado = i_pow(A, B);
    else if (rot == 1)
      Resultado = i_sqr(A, B);
    else
      Resultado = i_lg(A, B);
  }
  else if (op == SQR) {
    if (rot == 0)
      Resultado = i_sqr(A, B);
    else if (rot == 1)
      Resultado = i_pow(A, B);
    else
      Resultado = i_lg(B, A);
  }
  else if (op == SIN) {
    if (rot == 0)
      Resultado = i_sin(A, B);
    else if (rot == 1)
      Resultado = i_arcs(A, B);
    else
      Resultado = i_csin(A, B);
  }
  else if (op == ARCS) {
    if (rot == 0)
      Resultado = i_arcs(A, B);
    else if (rot == 1)
      Resultado = i_sin(A, B);
    else
      Resultado = i_csin(B, A);
  }
  else if (op == COS) {
    if (rot == 0)
      Resultado = i_cos(A, B);
    else if (rot == 1)
      Resultado = i_arcc(A, B);
    else
      Resultado = i_ccos(A, B);
  }
  else if (op == ARCC) {
    if (rot == 0)
      Resultado = i_arcc(A, B);
    else if (rot == 1)
      Resultado = i_cos(A, B);
    else
      Resultado = i_ccos(B, A);
  }
  else if (op == TAN) {
    if (rot == 0)
      Resultado = i_tan(A, B);
    else if (rot == 1)
      Resultado = i_arct(A, B);
    else
      Resultado = i_ctan(A, B);
  }
  else if (op == ARCT) {
    if (rot == 0)
      Resultado = i_arct(A, B);
    else if (rot == 1)
      Resultado = i_tan(A, B);
    else
      Resultado = i_ctan(B, A);
  }
  else if (op == EXP) {
    if (rot == 0)
      Resultado = i_exp(A, B);
    else if (rot == 1)
      Resultado = i_log(A, B);
    else
      Resultado = i_cexp(A, B);
  }
  else if (op == LOG) {
    if (rot == 0)
      Resultado = i_log(A, B);
    else if (rot == 1)
      Resultado = i_exp(A, B);
    else
      Resultado = i_cexp(B, A);
  }
  else if (op == ABS) {
    if (rot == 0)
      Resultado = i_abs(A, B);
    else if (rot == 1)
      Resultado = i_iabs(A, B);
    else
      Resultado = i_cabs(A, B);
  }
  else if (op == SGN) {
    if (rot == 0)
      Resultado = i_sgn(A, B);
    else if (rot == 1)
      Resultado = i_val(A, B);
    else
      Resultado = i_csgn(A, B);
  }
  else if (op == MAX) {
    if (rot == 0)
      Resultado = i_Max(A, B);
    else if (rot == 1)
      Resultado = i_iMax(A, B);
    else
      Resultado = i_cMax(A, B);
  }
  else if (op == MIN) {
    if (rot == 0)
      Resultado = i_Min(A, B);
    else if (rot == 1)
      Resultado = i_iMin(A, B);
    else
      Resultado = i_cMin(A, B);
  }
  return Resultado;
}

//TODO: verificar essa função
int igualdade_no_arco(Rest R, int rot) {

  intervalo Cabeca;
  lista Resultado;
  no n;
  int retorno = 0;

  if (rot == 0) {
    Cabeca = R->X->I;
    Resultado = opera(R->Y->I, R->Z->I, R->op, 0);
  }
  else if (rot == 1) {
    Cabeca = R->Y->I;
    Resultado = opera(R->X->I, R->Z->I, R->op, 1);
  }
  else {
    Cabeca = R->Z->I;
    Resultado = opera(R->X->I, R->Y->I, R->op, 2);
  }

  n = primeiro_no(Resultado);
  if (n && objeto(n) && proximo_no(n) == NULL) {
    intervalo res = objeto(n);
    if (Te(res) == Te(Cabeca) && Td(res) == Td(Cabeca) &&
        r_cmp(inf(res), "==", inf(Cabeca), NULL) && r_cmp(sup(res), "==", sup(Cabeca), NULL))
      retorno = 1;
  }

  destroi_lista(Resultado, destroi_intervalo);
  return retorno;
}

//--------------------------------------------------------------------------
// Imprime Restrições Rotacionadas

void imprime_rotacao(lista backtrack, int gera_grafo, FILE *f) {

  no n;
  PointerRest Pr;

  for (n = ultimo_no(backtrack); n; n = anterior_no(n)) {
    Pr = objeto(n);
    Rest R = Pr->rest;

    if (Pr->ja_buscada) {
      if (Pr->rotacao == 0) {
        fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->X->nome, R->Y->nome, opcode2str(R->op), R->Z->nome);
      }
      else if (Pr->rotacao == 1) {
        if (R->op == OP)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "IOP", R->Z->nome);
		else if (R->op == ADD)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "SUB", R->Z->nome);
		else if (R->op == MUL)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "DIV", R->Z->nome);
		else if (R->op == POW)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "SQR", R->Z->nome);
		else if (R->op == SIN)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "ARCS", R->Z->nome);
		else if (R->op == COS)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "ARCC", R->Z->nome);
		else if (R->op == TAN)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "ARCT", R->Z->nome);
		else if (R->op == EXP)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "LOG", R->Z->nome);
		else if (R->op == ABS)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "IABS", R->Z->nome);
		else if (R->op == SGN)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "VAL", R->Z->nome);
		else if (R->op == MAX)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "IMAX", R->Z->nome);
		else if (R->op == MIN)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "IMIN", R->Z->nome);

   	    else if (R->op == SUB)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "ADD", R->Z->nome);
		else if (R->op == DIV)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "MUL", R->Z->nome);
		else if (R->op == SQR)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "POW", R->Z->nome);
		else if (R->op == ARCS)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "SIN", R->Z->nome);
		else if (R->op == ARCC)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "COS", R->Z->nome);
		else if (R->op == ARCT)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "TAN", R->Z->nome);
		else if (R->op == LOG)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Y->nome, R->X->nome, "EXP", R->Z->nome);

		else {
		  printf("\n\n!!!!!!!!!!!!!!!ERRO!!!!!!!!!!!!!!!!!!!!!\n\n");
		  fprintf(f, "\n\n!!!!!!!!!!!!!!!ERRO!!!!!!!!!!!!!!!!!!!!!\n\n");
		}
      }
      else {
		if (R->op == OP)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "COP", R->Y->nome);
		else if (R->op == ADD)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "SUB", R->Y->nome);
		else if (R->op == MUL)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "DIV", R->Y->nome);
		else if (R->op == POW)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "LG", R->Y->nome);
		else if (R->op == SIN)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "CSIN", R->Y->nome);
		else if (R->op == COS)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "CCOS", R->Y->nome);
		else if (R->op == TAN)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "CTAN", R->Y->nome);
		else if (R->op == EXP)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "CEXP", R->Y->nome);
		else if (R->op == ABS)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "CABS", R->Y->nome);
		else if (R->op == SGN)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "CSGN", R->Y->nome);
		else if (R->op == MAX)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "CMAX", R->Y->nome);
		else if (R->op == MIN)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->X->nome, "CMIN", R->Y->nome);

		else if (R->op == SUB)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->Y->nome, "SUB", R->X->nome);
		else if (R->op == DIV)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->Y->nome, "DIV", R->X->nome);
		else if (R->op == SQR)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->Y->nome, "LG", R->X->nome);
		else if (R->op == ARCS)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->Y->nome, "CSIN", R->X->nome);
		else if (R->op == ARCC)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->Y->nome, "CCOS", R->X->nome);
		else if (R->op == ARCT)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->Y->nome, "CTAN", R->X->nome);
		else if (R->op == LOG)
		  fprintf(f, "c %d %s = %s %s %s\n", Pr->tipo, R->Z->nome, R->Y->nome, "CEXP", R->X->nome);

		else {
		  printf("\n\n!!!!!!!!!!!!!!!ERRO!!!!!!!!!!!!!!!!!!!!!\n\n");
		  fprintf(f, "\n\n!!!!!!!!!!!!!!!ERRO!!!!!!!!!!!!!!!!!!!!!\n\n");
		}
      }
    }
  }
  if (!gera_grafo) fprintf(f, "f\n");
}
