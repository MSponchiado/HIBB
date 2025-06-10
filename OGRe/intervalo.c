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

#include "intervalo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double DELTA = 0.1;
int ESCOLHA_ALEATORIA_INDE = FALSE;

//--------------------------------------------------------------------------

intervalo cria_intervalo(int e, int d, double a, double b) {

  intervalo X = (intervalo) malloc(sizeof(t_intervalo));
  if (X) {
    X->T_e = e;
    X->T_d = d;
    X->a = a;
    X->b = b;
  }
  else
    printf("ERRO: Impossível criar intervalo\n");

  return X;
}

//--------------------------------------------------------------------------

void destroi_intervalo(void *X) {
  free(X);
}

//--------------------------------------------------------------------------

intervalo copia_intervalo(intervalo X) {
  
  return cria_intervalo(Te(X), Td(X), inf(X), sup(X));
}

//--------------------------------------------------------------------------

int Te(intervalo X) { return X->T_e; }

int Td(intervalo X) { return X->T_d; }

double inf(intervalo X) { return X->a; }

double sup(intervalo X) { return X->b; }

//--------------------------------------------------------------------------

int degenerado(intervalo X) {
  return (r_cmp(X->a, "==", sup(X), &(X->a)) && Te(X) && Td(X));
}

//--------------------------------------------------------------------------

int eh_vazio(intervalo X) {
  return ((r_cmp(X->a, "==", sup(X), &(X->a)) && Te(X)*Td(X) == 0) ||
          r_cmp(X->a, ">", sup(X), &(X->a)) || eh_infinito(inf(X)) || eh_minfinito(sup(X)));
}

//--------------------------------------------------------------------------

double comprimento(intervalo X) {
  return X->b - X->a;
}

//--------------------------------------------------------------------------

void imprime_intervalo(intervalo X) {
  
  if (eh_vazio(X))
    printf("0");
  else
    printf("%c%.15f, %.15f%c", Te(X)?'[':'(', inf(X), sup(X), Td(X)?']':')');
}

//--------------------------------------------------------------------------

int i_subset(intervalo X, intervalo Y) {
  
  if (r_cmp(X->a, ">", inf(Y), &(X->a)) || (r_cmp(X->a, "==", inf(Y), &(X->a)) && (Te(X) <= Te(Y))))
    if (r_cmp(X->b, "<", sup(Y), &(X->b)) || (r_cmp(X->b, "==", sup(Y), &(X->b)) && (Td(X) <= Td(Y))))
      return TRUE;

  return FALSE;
}

//--------------------------------------------------------------------------

// retorna o menor valor de X (que pode ser -inf)
double i_min(intervalo X) {
  
  if (Te(X) || eh_minfinito(inf(X)))
    return inf(X);
  
  double value = nextafter(r_add(inf(X), EPSILON), INFINITY); // pega o valor representado seguinte a inf(X)+EPSILON
                                                            // pois inf(X)+EPSILON "==" inf(X)
  
  // Se a tolerância de igualdade está sendo calculada utilizando erro relativo, então
  // o value calculado acima ainda será "==" inf(X). Nesse caso, calcula value também
  // relativo a inf(X).
  if (r_cmp(value, "==", inf(X), &value))
    value = nextafter(r_add(inf(X), r_mul(inf(X), EPSILON)), INFINITY);
  
  // Se o valor obtido não está no intervalo, então o mínimo desse intervalo será o seu maximo
  // TODO: e se X for aberto à direita?
  if (r_cmp(value, ">=", sup(X), &value))
    value = sup(X);
  
  return value;
}

//--------------------------------------------------------------------------

// retorna o maior valor de X (que pode ser +inf)
double i_max(intervalo X) {
  
  if (Td(X) || eh_infinito(sup(X)))
    return sup(X);
  
  double value = nextafter(r_sub(sup(X), EPSILON), -INFINITY);// pega o valor representado anterior a inf(X)-EPSILON
                                                            // pois sup(X)-EPSILON "==" sup(X)
  
  // Se a tolerância de igualdade está sendo calculada utilizando erro relativo, então
  // o value calculado acima ainda será "==" sup(X). Nesse caso, calcula value também
  // relativo a sup(X).
  if (r_cmp(value, "==", sup(X), &value))
    value = nextafter(r_sub(sup(X), r_mul(sup(X), EPSILON)), -INFINITY);

  // Se o valor obtido não está no intervalo, então o mínimo desse intervalo será o seu maximo
  // TODO: e se X for aberto à esquerda?
  if (r_cmp(value, "<=", inf(X), &value))
    value = inf(X);
  
  return value;
}

//--------------------------------------------------------------------------
// X <- X cap Y
// retorna 1 se alterou X e 0 c.c.

int i_cap(intervalo X, intervalo Y) {

  int alterou_inf = TRUE, alterou_sup = TRUE;
  
  if (r_cmp(X->a, "<", inf(Y), &(X->a))) {
    X->a = inf(Y);
    X->T_e = Te(Y);
  }
  else if (r_cmp(X->a, "==", inf(Y), &(X->a)) && Te(X) > Te(Y)) {
    X->T_e = 0;
  }
  else {
    alterou_inf = FALSE;
  }

  if (r_cmp(X->b, ">", sup(Y), &(X->b))) {
    X->b = sup(Y);
    X->T_d = Td(Y);
  }
  else if (r_cmp(X->b, "==", sup(Y), &(X->b)) && Td(X) > Td(Y)) {
    X->T_d = 0;
  }
  else {
    alterou_sup = FALSE;
  }
  
  if (eh_vazio(X))
    X->T_e = X->T_d = X->a = X->b = 0;

  return alterou_inf + alterou_sup;
}

//--------------------------------------------------------------------------

intervalo i_cup(intervalo X, intervalo Y) {

  int e, d;
  double a, b;
  
  if (r_cmp(X->a, "<", inf(Y), &(X->a))) {
    a = inf(X);
    e = Te(X);
  }
  else if (r_cmp(X->a, ">", inf(Y), &(X->a))) {
    a = inf(Y);
    e = Te(Y);
  }  
  else {
    a = inf(X);
    e = (Te(X) || Te(Y)) ? 1 : 0;
  }
  
  if (r_cmp(X->b, ">", sup(Y), &(X->b))) {
    b = sup(X);
    d = Td(X);
  }
  else if (r_cmp(X->b, "<", sup(Y), &(X->b))) {
    b = sup(Y);
    d = Td(Y);
  }
  else {
    b = sup(X);
    d = (Td(X) || Td(Y)) ? 1 : 0;
  }

  intervalo Z = cria_intervalo(e, d, a, b);

  if (eh_vazio(Z))
    Z->T_e = Z->T_d = Z->a = Z->b = 0;

  return Z;
}

//--------------------------------------------------------------------------

void real_valuation(intervalo X, char pos) {
  
  double value;
  
  if (pos == 'a') {
    value = i_min(X);
  }
  else if (pos == 'c') {
    value = i_max(X);
  }
  else { // pos == 'b'
    if (ESCOLHA_ALEATORIA_INDE) {
      double min = i_min(X);
      double max = i_max(X);

      if (eh_minfinito(min)) min = -MAX_REAL;
      if (eh_infinito(max)) max = MAX_REAL;

      value = min + ((double)rand()/(double)(RAND_MAX)) * (max - min);
    }
    else {
      // pega o valor central deste intervalo
      if (eh_minfinito(inf(X))) {
        if (r_cmp(X->b, ">", 0, &(X->b)))
          value = 0;
        else if (r_cmp(X->b, "==", 0,&(X->b)))
          value = -MAX_REAL_DIV_2;
        else
          value = r_mul(sup(X), 2);
      }
      else if (eh_infinito(sup(X))) {
        if (r_cmp(X->a, "<", 0, &(X->a)))
          value = 0;
        else if (r_cmp(X->a, "==", 0, &(X->a)))
          value = MAX_REAL_DIV_2;
        else
          value = r_mul(inf(X), 2);
      }
      else {
        value = r_div(r_add(inf(X), sup(X)), 2); // Se o intervalo for pequeno, pode ser que dê erro, Nesse caso:
        if (r_cmp(value, "==", inf(X), &value) || r_cmp(value, "==", sup(X), &value)) {
          if (Te(X))
            value = inf(X);
          else if (Td(X))
            value = sup(X);
          else
            value = i_min(X);
        }
      }
    }
  }
  
  X->a = X->b = value;
  X->T_e = X->T_d = 1;
}

//--------------------------------------------------------------------------

void particiona(intervalo I, char lado, double delta) {
  
  double value;
  
  //printf("-> %c, %f e ", lado, DELTA);
  //imprime_intervalo(I);
  //printf("\n");
  
  // Se |I| <= DELTA (tamanho de grao), então não pode particionar
  if (r_cmp(r_abs(r_sub(inf(I), sup(I))), "<=", delta, NULL)) {
    value = INFINITY;
  }
  // Se I = [-inf, x] então não é possível pegar o valor médio.
  // Nesse caso pega 0, se x > 0, o dobro de x, se x < 0
  // (até um limite máximo MAX_REAL), ou MAX_REAL_DIV_2, se x = 0.
  // O caso análogo ocorre para I = [x, inf].
  else if (eh_minfinito(inf(I))) {
    if (r_cmp(I->b, ">", 0, &(I->b)))
      value = 0;
    else if (r_cmp(I->b, "==", 0, &(I->b)))
      value = -MAX_REAL_DIV_2;
    else {
      value = r_mul(sup(I), 2);
      if (r_cmp(value, "<", -MAX_REAL, &value) && lado == 'a')
        value = INFINITY;
    }
  }
  else if (eh_infinito(sup(I))) {
    if (r_cmp(I->a, "<", 0, &(I->a)))
      value = 0;
    else if (r_cmp(I->a, "==", 0, &(I->a)))
      value = MAX_REAL_DIV_2;
    else {
      value = r_mul(inf(I), 2);
      if (r_cmp(value, ">", MAX_REAL, &value) && lado == 'b')
        value = INFINITY;
    }
  }
  else {
    value = (inf(I) + sup(I))/2;
  }
  
  if (eh_infinito(value)) {
    I->a = I->b = 0;
    I->T_e = I->T_d = 0;
  }
  else if (lado == 'a') {
    I->b = value;
    I->T_d = 1;
  }
  else if (lado == 'b') {
    I->a = value;
    I->T_e = 0;
  }
  
  //printf("<-\n");
}

//--------------------------------------------------------------------------
// Considerando que M e N são listas de intervalos: M <- M U N,
// de forma que não tenha intervalos redundantes, e destroi lista N
void merge_ordenado(lista M, lista N) {
  
  no r, s, t;
  intervalo I;
  
  for (r = primeiro_no(N); r; r = proximo_no(r)) {
    
    if (!eh_vazio(objeto(r))) {
      // verifica se existe algum intervalo s em M com limitante superior
      // maior ou igual que o limitante inferior do intervalo atual r
      
      s = primeiro_no(M);
      intervalo Is;
      while (s && (Is = objeto(s)) && r_cmp(Is->b, "<", inf(objeto(r)), &(Is->b)))
        s = proximo_no(s);
      
      // no caso positivo, pode haver uma interseção, isto é, redundância,
      // ou encontramos a posição de r na ordem certa
      if (s) {
        
        // Se o limitante inferior deste intervalo s for maior que o superior
        // do intervalo atual r, então, de fato, não há redundância, apenas
        // o inserimos na posição correta
        Is = objeto(s);
        if (r_cmp(Is->a, ">", sup(objeto(r)), &(Is->a)) ||
            (r_cmp(Is->a, "==", sup(objeto(r)), &(Is->a)) && !Te(objeto(s)) && !Td(objeto(r))))
          insere(objeto(r), s, M);
        
        // Senão, temos uma redundância. Substituímos s pela união deste com r
        else {
          I = i_cup(objeto(r), objeto(s));
          destroi_intervalo(objeto(r));
          destroi_intervalo(objeto(s));
          s->obj = I;
          
          // Além disso, r poderia ser uma "ponte" que conecta s ao(s)
          // próximo(s) intervalos que já estão em M.
          t = proximo_no(s);
          intervalo It;
          while (t && (It = objeto(t)) && (r_cmp(It->a, "<", sup(objeto(s)), &(It->a)) ||
                       (r_cmp(It->a, "==", sup(objeto(s)), &(It->a)) && (Te(objeto(t)) || Td(objeto(s)))))) {
            
            // redundância
            I = i_cup(objeto(t), objeto(s));
            remove_no(t, M, destroi_intervalo);
            destroi_intervalo(objeto(s));
            s->obj = I;
            t = proximo_no(s);
          }
        }
      }
      // no caso negativo, o intervalo r vai no final da lista de intervalos M
      else
        enfileira(objeto(r), M);
    }
    else
      destroi_intervalo(objeto(r));
  }
  destroi_lista(N, nao_destroi);
}

//--------------------------------------------------------------------------
//
// AQUI COMECAM AS OPERACOES INTERVALARES TERNARIAS
//
//--------------------------------------------------------------------------

// NEUTRO

//--------------------------------------------------------------------------
lista i_neutro(intervalo A, intervalo B) {
  intervalo X;
  
  if (eh_vazio(A) || eh_vazio(B))
    X = cria_intervalo(0, 0, 0, 0);
  else
    X = cria_intervalo(0, 0, -INFINITY, INFINITY);

  lista M = cria_lista();
  enfileira(X, M);
  
  return M;
}
//--------------------------------------------------------------------------

// SOMA

//--------------------------------------------------------------------------
lista i_add(intervalo A, intervalo B) {

  int e, d;
  double a, b;

  if (eh_vazio(A) || eh_vazio(B))
    a = b = e = d = 0;

  else {
    a = r_add(inf(A), inf(B));
    b = r_add(sup(A), sup(B));

    e = Te(A)*Te(B);
    d = Td(A)*Td(B);
  }

  intervalo Z = cria_intervalo(e, d, a, b);

  if (eh_vazio(Z))
    Z->T_e = Z->T_d = Z->a = Z->b = 0;

  lista M = cria_lista();
  enfileira(Z, M);
  
  return M;
}
//--------------------------------------------------------------------------
lista i_add_inv(intervalo A, intervalo B) {
  return i_add(B, A);
}
//--------------------------------------------------------------------------
lista i_sub(intervalo A, intervalo B) {

  int e, d;
  double a, b;

  if (eh_vazio(A) || eh_vazio(B))
    a = b = e = d = 0;

  else {
    a = r_sub(inf(A), sup(B));
    b = r_sub(sup(A), inf(B));
    
    e = Te(A)*Td(B);
    d = Td(A)*Te(B);
  }

  intervalo Z = cria_intervalo(e, d, a, b);

  if (eh_vazio(Z))
    Z->T_e = Z->T_d = Z->a = Z->b = 0;

  lista M = cria_lista();
  enfileira(Z, M);
  
  return M;
}
//--------------------------------------------------------------------------
lista i_sub_inv(intervalo A, intervalo B) {
  return i_sub(B, A);
}
//--------------------------------------------------------------------------

// PRODUTO

//--------------------------------------------------------------------------
lista i_mul(intervalo A, intervalo B) {

  int e, d;
  double a, b, tmp;
  
  if (eh_vazio(A) || eh_vazio(B))
    a = b = e = d = 0;

  else {
    
    // a = min{inf(A)*inf(B), inf(A)*sup(B), sup(A)*inf(B), sup(A)*sup(B)}
    // b = max{inf(A)*inf(B), inf(A)*sup(B), sup(A)*inf(B), sup(A)*sup(B)}
    a = r_mul(inf(A), inf(B));
    b = r_mul(inf(A), inf(B));
    e = Te(A)*Te(B);
    d = Te(A)*Te(B);

    tmp = r_mul(inf(A), sup(B));
    if (r_cmp(tmp, "<", a, &tmp)) {
      a = tmp;
      e = Te(A)*Td(B);
    }
    if (r_cmp(tmp, ">", b, &tmp)) {
      b = tmp;
      d = Te(A)*Td(B);
    }
    
    tmp = r_mul(sup(A), inf(B));
    if (r_cmp(tmp, "<", a, &tmp)) {
      a = tmp;
      e = Td(A)*Te(B);
    }
    if (r_cmp(tmp, ">", b, &tmp)) {
      b = tmp;
      d = Td(A)*Te(B);
    }
    
    tmp = r_mul(sup(A), sup(B));
    if (r_cmp(tmp, "<", a, &tmp)) {
      a = tmp;
      e = Td(A)*Td(B);
    }
    if (r_cmp(tmp, ">", b, &tmp)) {
      b = tmp;
      d = Td(A)*Td(B);
    }
  }

  // teoricamente, [0,2] x (1,3] = [0,6]
  // mas aqui ele ta resultando em (0,6]
  // Por outro lado, (0,2) x (1,3] = (0,6] está correto
  // Logo, se 0 pertence a A ou B, então 0 também deve pertencer a A * B
  if ((r_cmp(A->a, "==", 0, &(A->a)) && Te(A)) ||
      (r_cmp(A->b, "==", 0, &(A->b)) && Td(A)) ||
      (r_cmp(B->a, "==", 0, &(B->a)) && Te(B)) ||
      (r_cmp(B->b, "==", 0, &(B->b)) && Td(B))) {

    if (r_cmp(a, "==", 0, &a))
      e = 1;
    if (r_cmp(b, "==", 0, &b))
      d = 1;
  }

  intervalo Z = cria_intervalo(e, d, a, b);
  
  if (eh_vazio(Z))
    Z->T_e = Z->T_d = Z->a = Z->b = 0;

  lista M = cria_lista();
  enfileira(Z, M);
  
  return M;
}
//--------------------------------------------------------------------------
lista i_mul_inv(intervalo A, intervalo B) {
  return i_mul(B, A);
}
//--------------------------------------------------------------------------
lista i_div(intervalo A, intervalo B) {
  
  int e, d;
  double a, b, tmp;
  intervalo Y;
  lista M, N;

  // se operando é vazio
  if (eh_vazio(A) || eh_vazio(B)) {
    
    M = cria_lista();
    enfileira(cria_intervalo(0, 0, 0, 0), M);
  }
  
  // Pensando na consistência, se B = [0,0], então A só pode ser [0,0] na restrição Z = A / B
  // pois B * Z = A só é verdade para 0 * Z = 0. No entanto, Z pode ser qualquer coisa.
  // Portanto, se B = [0,0] então A / B = [-oo,oo].
  // Além disso, se 0 está em ambos A e B, então há divisão por zero nesse caso também.
  else if ((r_cmp(B->a, "==", 0, &(B->a)) && r_cmp(B->b, "==", 0, &(B->b))) ||
           (r_cmp(A->a, "<=", 0, &(A->a)) && r_cmp(A->b, ">=", 0, &(A->b)) &&
            r_cmp(B->a, "<=", 0, &(B->a)) && r_cmp(B->b, ">=", 0, &(B->b))))
  {
    
    M = cria_lista();
    enfileira(cria_intervalo(0, 0, -INFINITY, INFINITY), M);
  }
  
  // denominador inclui zero (não nos limitantes), isto é, b1 < 0 < b2:
  // A / [b1, b2] = A / <[b1, 0], [0, b2]>
  else if (r_cmp(B->a, "<", 0, &(B->a)) && r_cmp(B->b, ">", 0, &(B->b))) {
    
    Y = cria_intervalo(Te(B), 1, inf(B), 0);
    M = i_div(A, Y);
    destroi_intervalo(Y);
    Y = cria_intervalo(1, Td(B), 0, sup(B));
    N = i_div(A, Y);
    destroi_intervalo(Y);
    merge_ordenado(M, N);
  }
  
  // denominador não contém zero ou contém nos limitantes
  else {
    
    // Assert
    if (eh_minfinito(inf(B)))
      B->T_e = 0;
    if (eh_infinito(sup(B)))
      B->T_d = 0;
    
    // se B = [b1, 0] o resultado será [-inf, x]. Logo, a divisão por zero
    // deve resultar -inf ao invés de inf.
    // No mais, é igual a multiplicação:
    // a = min{inf(A)/inf(B), inf(A)/sup(B), sup(A)/inf(B), sup(A)/sup(B)}
    // b = max{inf(A)/inf(B), inf(A)/sup(B), sup(A)/inf(B), sup(A)/sup(B)}
    a = r_div(inf(A), sup(B));
    if (r_cmp(B->b, "==", 0, &(B->b))) a = -a;
    b = r_div(inf(A), sup(B));
    if (r_cmp(B->b, "==", 0, &(B->b))) b = -b;
    e = Te(A)*Td(B);
    d = Te(A)*Td(B);

    tmp = r_div(inf(A), inf(B));
    if (r_cmp(tmp, "<", a, &tmp)) {
      a = tmp;
      e = Te(A)*Te(B);
    }
    if (r_cmp(tmp, ">", b, &tmp)) {
      b = tmp;
      d = Te(A)*Te(B);
    }

    tmp = r_div(sup(A), sup(B));
    if (r_cmp(B->b, "==", 0, &(B->b)))
      tmp = -tmp;
    if (r_cmp(tmp, "<", a, &tmp)) {
      a = tmp;
      e = Td(A)*Td(B);
    }
    if (r_cmp(tmp, ">", b, &tmp)) {
      b = tmp;
      d = Td(A)*Td(B);
    }

    tmp = r_div(sup(A), inf(B));
    if (r_cmp(tmp, "<", a, &tmp)) {
      a = tmp;
      e = Td(A)*Te(B);
    }
    if (r_cmp(tmp, ">", b, &tmp)) {
      b = tmp;
      d = Td(A)*Te(B);
    }

    if ((r_cmp(A->a, "==", 0, &(A->a)) && Te(A)) ||
        (r_cmp(A->b, "==", 0, &(A->b)) && Td(A)) ||
        (r_cmp(B->a, "==", 0, &(B->a)) && Te(B)) ||
        (r_cmp(B->b, "==", 0, &(B->b)) && Td(B))) {
      
      if (r_cmp(a, "==", 0, &a))
        e = 1;
      if (r_cmp(b, "==", 0, &b))
        d = 1;
    }

    intervalo Z = cria_intervalo(e, d, a, b);
    
    if (eh_vazio(Z))
      Z->T_e = Z->T_d = Z->a = Z->b = 0;

    M = cria_lista();
    enfileira(Z, M);
  }

  return M;
}
//--------------------------------------------------------------------------
lista i_div_inv(intervalo A, intervalo B) {
  return i_div(B, A);
}
//--------------------------------------------------------------------------

// POTENCIACAO

//--------------------------------------------------------------------------
lista i_pow(intervalo A, intervalo B) {
  
  intervalo X = NULL;
  intervalo Y, UM;
  lista M = NULL, N = NULL;

  int exp = (int)inf(B);
  
  // operando vazio
  if (eh_vazio(A) || eh_vazio(B))
   X = cria_intervalo(0, 0, 0, 0);
  
  // expoente negativo
  else if (exp < 0) {
    
    UM = cria_intervalo(1, 1, 1, 1);
    Y = cria_intervalo(1, 1, -exp, -exp);
    N = i_pow(A, Y);                        // como i_pow para expoente positivo sempre devolve
    M = i_div(UM, objeto(primeiro_no(N)));  // um único intervalo, então basta pegar o objeto do 1o nó
    
    destroi_intervalo(UM);
    destroi_intervalo(Y);
    destroi_lista(N, destroi_intervalo);
  }
  
  // expoente nulo
  else if (exp == 0)
    X = cria_intervalo(1, 1, 1, 1);
  
  // expoente positivo e ímpar
  else if (exp % 2)
    X = cria_intervalo(Te(A), Td(A), r_pow(inf(A), exp), r_pow(sup(A), exp));
  
  // expoente positivo e par e A = [x, y] para x >= 0
  else if (r_cmp(A->a, ">=", 0, &(A->a)))
    X = cria_intervalo(Te(A), Td(A), r_pow(inf(A), exp), r_pow(sup(A), exp));
  
  // expoente positivo e par e A = [x, y] para y < 0
  else if (r_cmp(A->b, "<", 0, &(A->b)))
    X = cria_intervalo(Te(A), Td(A), r_pow(sup(A), exp), r_pow(inf(A), exp));
  
  // expoente positivo e par e A = [x,y] para x < 0 < y
  else {
    double r1 = r_pow(inf(A), exp);
    double r2 = r_pow(sup(A), exp);
    
    if (r_cmp(r1, ">", r2, &r1))
      X = cria_intervalo(1, Te(A), 0, r1);
    else
      X = cria_intervalo(1, Td(A), 0, r2);
  }
  
  // só entra no if abaixo se exp >= 0
  if (X) {
    if (eh_vazio(X))
      X->T_e = X->T_d = X->a = X->b = 0;
    M = cria_lista();
    enfileira(X, M);
  }
  
  return M;
}
//--------------------------------------------------------------------------
lista i_pow_inv(intervalo A, intervalo B) {
  return i_pow(B, A);
}
//--------------------------------------------------------------------------
lista i_sqr(intervalo A, intervalo B) {
  
  intervalo X = NULL;
  intervalo Y = NULL;
  intervalo Z, UM;
  
  lista M = NULL, N = NULL;
  
  int ind = (int)inf(B);
  
  // operando vazio
  if (eh_vazio(A) || eh_vazio(B))
    X = cria_intervalo(0, 0, 0, 0);
  
  // indice negativo
  else if (ind < 0) {
    
    UM = cria_intervalo(1, 1, 1, 1);
    Z = cria_intervalo(1, 1, -ind, -ind);
    N = i_sqr(A, Z); // N pode conter 2 intervalos

    M = cria_lista();
    for (no p = primeiro_no(N); p; p = proximo_no(p))
      merge_ordenado(M, i_div(UM, objeto(p)));

    destroi_intervalo(UM);
    destroi_intervalo(Z);
    destroi_lista(N, destroi_intervalo);
  }
  
  // indice nulo
  else if (ind == 0)
    X = cria_intervalo(0, 0, -INFINITY, INFINITY);
  
  // indice positivo e ímpar
  else if (ind % 2)
    X = cria_intervalo(Te(A), Td(A), r_sqr(inf(A), ind), r_sqr(sup(A), ind));
  
  // indice positivo e par e A = [x, y] para x >= 0
  else if (r_cmp(A->a, ">=", 0, &(A->a))) {
    X = cria_intervalo(Td(A), Te(A), -r_sqr(sup(A), ind), -r_sqr(inf(A), ind));
    Y = cria_intervalo(Te(A), Td(A), r_sqr(inf(A), ind), r_sqr(sup(A), ind));
  }

  // indice positivo e par e A = [x, y] para y < 0
  else if (r_cmp(A->b, "<", 0, &(A->b)))
    X = cria_intervalo(0, 0, 0, 0);
  
  // indice positivo e par e A = [x,y] para x < 0 < y
  else
    X = cria_intervalo(Td(A), Td(A), -r_sqr(sup(A), ind), r_sqr(sup(A), ind));
  
  // só vai entrar aqui se ind >= 0
  if (X) {
    if (eh_vazio(X))
      X->T_e = X->T_d = X->a = X->b = 0;
    
    M = cria_lista();
    enfileira(X, M);
    
    if (Y) {
      if (eh_vazio(Y))
        Y->T_e = Y->T_d = Y->a = Y->b = 0;
      
      N = cria_lista();
      enfileira(Y, N);
      merge_ordenado(M, N);
    }
  }
  
  return M;
}
//--------------------------------------------------------------------------
lista i_sqr_inv(intervalo A, intervalo B) {
  return i_sqr(B, A);
}
//--------------------------------------------------------------------------
// Log eh operacao neutra, pois potencia e raiz são sempre com constantes
lista i_lg(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_lg_inv(intervalo A, intervalo B) {
  return i_lg(B, A);
}
//--------------------------------------------------------------------------

// VALOR ABSOLUTO
//
// X = Y abs  Z   =>  [-1,3] = [-2,1] abs  Z      => X = [-1,3] cap [0,2]  = [0,2]
// Y = X iabs Z   =>  [-2,1] = [0,2]  iabs Z      => Y = [-2,1] cap [-2,2] = [-2,1]
// Z = X cabs Y   =>       Z = [0,2]  cabs [-2,1] => Z = Z cap (-oo, +oo)  = Z			(neutro)
//
//--------------------------------------------------------------------------
lista i_abs(intervalo A, intervalo B) {

	intervalo X;

	if (eh_vazio(A) || eh_vazio(B))
		X = cria_intervalo(0, 0, 0, 0);
	else if (r_cmp(A->b, "<=", 0, &(A->b)))
		X = cria_intervalo(Td(A), Te(A), -sup(A), -inf(A));
	else if (r_cmp(A->a, "<=", 0, &(A->a))) {
		if (r_cmp(-(A->a), ">", A->b, &(A->b)))
			X = cria_intervalo(1, Te(A), 0, -inf(A));
		else
			X = cria_intervalo(1, Td(A), 0, sup(A));
	}
	else
		X = cria_intervalo(Te(A), Td(A), inf(A), sup(A));

	lista M = cria_lista();
	enfileira(X, M);

	return M;
}
//--------------------------------------------------------------------------
lista i_abs_inv(intervalo A, intervalo B) {
  return i_abs(B, A);
}
//--------------------------------------------------------------------------
lista i_iabs(intervalo A, intervalo B) {

	intervalo X, Y = NULL;

	if (eh_vazio(A) || eh_vazio(B))
		X = cria_intervalo(0, 0, 0, 0);			
	else if (r_cmp(A->a, "==", 0, &(A->a)) && Te(A))   // A = [a,b], a,b >= 0
		X = cria_intervalo(Td(A), Td(A), -sup(A), sup(A));
	else if (r_cmp(A->a, "==", 0, &(A->a)) && !Te(A)) {
		X = cria_intervalo(Td(A), 0, -sup(A), 0);
		Y = cria_intervalo(0, Td(A), 0, sup(A));
	}
	else {
		X = copia_intervalo(A);
		Y = cria_intervalo(Td(A), Te(A), -sup(A), -inf(A));
	}

	lista M = cria_lista();
	enfileira(X, M);
	if (Y) enfileira(Y, M);

	return M;
}
//--------------------------------------------------------------------------
lista i_iabs_inv(intervalo A, intervalo B) {
  return i_iabs(B, A);
}
//--------------------------------------------------------------------------
lista i_cabs(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_cabs_inv(intervalo A, intervalo B) {
  return i_cabs(B, A);
}
//--------------------------------------------------------------------------

// SINAL
//
// X = Y sgn  Z   =>  [-1,0] = [-2,1] sgn  Z      => X = [-1,0] cap {-1,0,1} = {-1,0}
// Y = X isgn Z   =>  [-2,1] = {-1,0} iabs Z      => Y = [-2,1] cap (-oo,0]  = [-2,0]
// Z = X csgn Y   =>       Z = {-1,0} cabs [-2,0] => Z = Z cap (-oo, +oo)    = Z			(neutro)
//
//--------------------------------------------------------------------------
lista i_sgn(intervalo A, intervalo B) {

	lista M = cria_lista();

	if (eh_vazio(A) || eh_vazio(B)){
		enfileira(cria_intervalo(0, 0, 0, 0), M);
		return M;
	}

	if (r_cmp(A->a, "<", 0, &(A->a)))
		enfileira(cria_intervalo(1, 1, -1, -1), M);
	if (r_cmp(A->a, "<", 0, &(A->a)) || (r_cmp(A->a, "==", 0, &(A->a)) && Te(A) == 1))
		if (r_cmp(A->b, ">", 0, &(A->b)) || (r_cmp(A->b, "==", 0, &(A->b)) && Td(A) == 1))
			enfileira(cria_intervalo(1, 1, 0, 0), M);
	if (r_cmp(A->b, ">", 0, &(A->b)))
		enfileira(cria_intervalo(1, 1, 1, 1), M);

	return M;
}
//--------------------------------------------------------------------------
lista i_sgn_inv(intervalo A, intervalo B) {
  return i_sgn(B, A);
}
//--------------------------------------------------------------------------
lista i_val(intervalo A, intervalo B) {

	intervalo X;

	if (eh_vazio(A) || eh_vazio(B))
		X = cria_intervalo(0, 0, 0, 0);
	else if (inf(A) == 0 && sup(A) == 0)
		X = cria_intervalo(1, 1, 0, 0);
	else if (inf(A) == 1 && sup(A) == 1)
		X = cria_intervalo(0, 0, 0, INFINITY);
	else if (inf(A) == -1 && sup(A) == -1)
		X = cria_intervalo(0 ,0, -INFINITY, 0);
	else
		X = cria_intervalo(0, 0, -INFINITY, INFINITY);

	lista M = cria_lista();
	enfileira(X, M);
	return M;
}
//--------------------------------------------------------------------------
lista i_val_inv(intervalo A, intervalo B) {
  return i_val(B, A);
}
//--------------------------------------------------------------------------
lista i_csgn(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_csgn_inv(intervalo A, intervalo B) {
  return i_csgn(B, A);
}
//--------------------------------------------------------------------------

// MAXIMO
//
// X = Y Max  Z   =>  [-1,0] = [-2,1] Max [-1,2] => X = [-1,0] cap [-1, 2] = [-1, 0]
// Y = X iMax Z   =>  nao esta claro o que isso deve fazer
// Z = X cMax Y   =>  nem isso
//
//--------------------------------------------------------------------------
lista i_Max(intervalo A, intervalo B) {

    intervalo X;

	if (eh_vazio(A) || eh_vazio(B))
		X = cria_intervalo(0, 0, 0, 0);

	else {
		double max_inf = inf(A);
		double max_sup = sup(A);
		int Tinf = Te(A);
		int Tsup = Td(A);

		if (r_cmp(B->a, ">", max_inf, &(B->a)) || (r_cmp(B->a, "==", max_inf, &(B->a)) && (Te(B) == 0))) {
			max_inf = inf(B);
			Tinf = Te(B);
		}
		if (r_cmp(B->b, ">", max_sup, &(B->b)) || (r_cmp(B->b, "==", max_sup, &(B->b)) && (Td(B) == 1))) {
			max_sup = sup(B);
			Tsup = Td(B);
		}
		X = cria_intervalo(Tinf, Tsup, max_inf, max_sup);
	}

	lista M = cria_lista();
	enfileira(X, M);

	return M;
}
//--------------------------------------------------------------------------
lista i_Max_inv(intervalo A, intervalo B) {
  return i_Max(B, A);
}
//--------------------------------------------------------------------------
lista i_iMax(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_iMax_inv(intervalo A, intervalo B) {
  return i_iMax(B, A);
}
//--------------------------------------------------------------------------
lista i_cMax(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_cMax_inv(intervalo A, intervalo B) {
  return i_cMax(B, A);
}
//--------------------------------------------------------------------------

// MINIMO
//
// X = Y Min  Z   =>  [-1,0] = [-2,1] Max [-1,2] => X = [-1,0] cap [-1, 2] = [-1, 0]
// Y = X iMin Z   =>  nao esta claro o que isso deve fazer
// Z = X cMin Y   =>  nem isso
//
//--------------------------------------------------------------------------
lista i_Min(intervalo A, intervalo B) {

    intervalo X;

	if (eh_vazio(A) || eh_vazio(B))
		X = cria_intervalo(0, 0, 0, 0);

	else {
		double min_inf = inf(A);
		double min_sup = sup(A);
		int Tinf = Te(A);
		int Tsup = Td(A);

		if (r_cmp(B->a, "<", min_inf, &(B->a)) || (r_cmp(B->a, "==", min_inf, &(B->a)) && (Te(B) == 1))) {
			min_inf = inf(B);
			Tinf = Te(B);
		}
		if (r_cmp(B->b, "<", min_sup, &(B->b)) || (r_cmp(B->b, "==", min_sup, &(B->b)) && (Td(B) == 0))) {
			min_sup = sup(B);
			Tsup = Td(B);
		}
		X = cria_intervalo(Tinf, Tsup, min_inf, min_sup);
	}

	lista M = cria_lista();
	enfileira(X, M);

	return M;
}
//--------------------------------------------------------------------------
lista i_Min_inv(intervalo A, intervalo B) {
  return i_Min(B, A);
}
//--------------------------------------------------------------------------
lista i_iMin(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_iMin_inv(intervalo A, intervalo B) {
  return i_iMin(B, A);
}
//--------------------------------------------------------------------------
lista i_cMin(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_cMin_inv(intervalo A, intervalo B) {
  return i_cMin(B, A);
}
//--------------------------------------------------------------------------

// EXPONENCIAL

//--------------------------------------------------------------------------
// exponencial de um intervalo (base constante positiva diferente de 1)
lista i_exp(intervalo A, intervalo B) {

	double base = inf(B);
    intervalo X;

	if (eh_vazio(A) || eh_vazio(B))
		X = cria_intervalo(0, 0, 0, 0);
	else if (base > 1) {
		double b = r_exp(base, sup(A));
		double d = eh_infinito(b) ? 0 : Td(A);
		X = cria_intervalo(Te(A), d, r_exp(base, inf(A)), b);
	}
	else {
		double b = r_exp(base, inf(A));
		double d = eh_infinito(b) ? 0 : Td(A);
		X = cria_intervalo(Te(A), d, r_exp(base, sup(A)), b);
	}
	lista M = cria_lista();
	enfileira(X, M);

	return M;
}
//--------------------------------------------------------------------------
lista i_exp_inv(intervalo A, intervalo B) {
  return i_exp(B, A);
}
//--------------------------------------------------------------------------
// o inverso de X = Y exp c = c^Y eh Y = X log c = log_c(X). Soh eh definido para X positivo
// e base constante positiva diferente de 1.
lista i_log(intervalo A, intervalo B) {

	double base = inf(B);
	intervalo X;
	double a, b;
	int e, d;

	if (eh_vazio(A) || eh_vazio(B))
		X = cria_intervalo(0, 0, 0, 0);
	else if (r_cmp(A->b, "<=", 0, &(A->b)))
		X = cria_intervalo(0, 0, 0, 0);
	else if (base > 1) {
		if (r_cmp(A->a, "<", 0, &(A->a)))
			a = r_log(base, 0);
		else
			a = r_log(base, inf(A));
		b = r_log(base, sup(A));		
		e = eh_minfinito(a) ? 0 : Te(A);
		d = eh_infinito(b) ? 0 : Td(A);
		X = cria_intervalo(e, d, a, b);
	}
	else {
		a = r_log(base, sup(A));
		if (r_cmp(A->a, "<", 0, &(A->a)))
			b = r_log(base, 0);
		else
			b = r_log(base, inf(A));
		e = eh_minfinito(a) ? 0 : Te(A);
		d = eh_infinito(b) ? 0 : Td(A);
		X = cria_intervalo(e, d, a, b);
	}
	lista M = cria_lista();
	enfileira(X, M);

	return M;
}
//--------------------------------------------------------------------------
lista i_log_inv(intervalo A, intervalo B) {
  return i_log(B, A);
}
//--------------------------------------------------------------------------
// o segundo inverso de X = exp(Y) eh e = sqrt[Y](X), que pode ser definido como neutro, pois e eh constante
lista i_cexp(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_cexp_inv(intervalo A, intervalo B) {
  return i_cexp(B, A);
}
//--------------------------------------------------------------------------
//
// SENO, COSSENO E TANGENTE
//
// TODO: acho que essas operacoes nao admitem inversos na algebra intervalar.
//--------------------------------------------------------------------------
// funcao base para seno e cosseno intervalar
intervalo f(intervalo A, double fa, double fb) {
	return cria_intervalo(Te(A), Td(A), fa, fb);
}
//--------------------------------------------------------------------------
// funcao base para seno e cosseno intervalar
intervalo f_max(intervalo A, double fa, double fb) {

	double m;
	int t;

	if (r_cmp(fa, ">", fb, &fa)) {
		t = Te(A);
		m = fa;
	}
	else if (r_cmp(fa, "==", fb, &fa) && (Te(A) || Td(A))){
		t = 1;
		m = fa;
	}
	else {
		t = Td(A);
		m = fb;
	}
	return cria_intervalo(1, t, -1, m);
}
//--------------------------------------------------------------------------
// funcao base para seno e cosseno intervalar
intervalo f_min(intervalo A, double fa, double fb) {

	double m;
	int t;

	if (r_cmp(fa, "<", fb, &fa)) {
		t = Te(A);
		m = fa;
	}
	else if (r_cmp(fa, "==", fb, &fa) && (Te(A) || Td(A))){
		t = 1;
		m = fa;
	}
	else {
		t = Td(A);
		m = fb;
	}
	return cria_intervalo(t, 1, m, 1);
}
//--------------------------------------------------------------------------
// funcao coringa para seno e cosseno
lista i_sincos(intervalo A, int sin) {

	intervalo X;

	double x = r_div(inf(A), M_PI); //TODO: e se essa divisao nao for definida?
	double y = r_div(sup(A), M_PI);

	double fa, fb;
	if (sin) {
		fa = r_sen(inf(A));
		fb = r_sen(sup(A));
	}
	else {
		fa = r_cos(inf(A));
		fb = r_cos(sup(A));
	}

	// ponto de referencia (em PIs) no grafico: dependendo do ponto de referencia
	// deve-se marcar o inicio como 1, 0.5 ou 1.5 PIs anteriores.
	double ref = floor(x);
	if (sin) {
		if (((int)x) % 2 == 1)
			ref -= 1;
	}
	else {
		if (((int)x) % 2 == 0)
			ref -= 0.5;
		else
			ref -= 1.5;
	}

	// todos os casos no grafico:
	double comprimento = r_sub(sup(A), inf(A));		// TODO: nao estou usando r_cmp nas comparacoes internas deste trecho

	if (r_cmp(comprimento, ">=", 2*M_PI, &comprimento)) {
        X = cria_intervalo(1, 1, -1, 1);
	}
    else if (r_cmp(comprimento, ">=", 1.5*M_PI, &comprimento)) {
        if (x > ref+0.5 && y <= ref+2.5)
			X = f_max(A, fa, fb);
        else if (x > ref+1.5 && y <= ref+3.5)
			X = f_min(A, fa, fb);
        else
            X = cria_intervalo(1, 1, -1, 1);
	}
    else if (r_cmp(comprimento, ">=", 1*M_PI, &comprimento)) {
        if (y <= ref+1.5)
            X = f_min(A, fa, fb);
        else if (x <= ref+0.5)
            X = cria_intervalo(1, 1, -1, 1);
        else if (y <= ref+2.5)
            X = f_max(A, fa, fb);
        else if (x <= ref+1.5)
            X = cria_intervalo(1, 1, -1, 1);
        else if (y <= ref+3.5)
            X = f_min(A, fa, fb);
        else
            X = cria_intervalo(1, 1, -1, 1);
	}
    else if (r_cmp(comprimento, ">=", 0.5*M_PI, &comprimento)) {
        if (x <= ref+0.5)
            X = f_min(A, fa, fb);
        else if (y <= ref+1.5)
			X = f(A, fa, fb);
        else if (x <= ref+1.5)
            X = f_max(A, fa, fb);
        else if (y <= ref+2.5)
            X = f(A, fa, fb);
        else
            X = f_min(A, fa, fb);
	}
    else {
        if (y <= ref+0.5)
            X = f(A, fa, fb);
        else if (x <= ref+0.5)
            X = f_min(A, fa, fb);
        else if (y <= ref+1.5)
            X = f(A, fa, fb);
        else if (x <= ref+1.5)
            X = f_max(A, fa, fb);
        else if (y <= ref+2.5)
            X = f(A, fa, fb);
        else
            X = f_min(A, fa, fb);
	}

	if (eh_vazio(X)) {
		destroi_intervalo(X);
		X = cria_intervalo(0, 0, 0, 0);
	}

	lista M = cria_lista();
	enfileira(X, M);

	return M;
}
//--------------------------------------------------------------------------
// seno
lista i_sin(intervalo A, intervalo B) {

	if (eh_vazio(A) || eh_vazio(B)) {
		lista M = cria_lista();
		enfileira(cria_intervalo(0, 0, 0, 0), M);
		return M;
	}
	return i_sincos(A, TRUE);
}
//--------------------------------------------------------------------------
lista i_sin_inv(intervalo A, intervalo B) {
  return i_sin(B, A);
}
//--------------------------------------------------------------------------
lista i_arcs(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_arcs_inv(intervalo A, intervalo B) {
  return i_arcs(B, A);
}
//--------------------------------------------------------------------------
lista i_csin(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_csin_inv(intervalo A, intervalo B) {
  return i_csin(B, A);
}
//--------------------------------------------------------------------------
// cosseno
lista i_cos(intervalo A, intervalo B) {

	if (eh_vazio(A) || eh_vazio(B)) {
		lista M = cria_lista();
		enfileira(cria_intervalo(0, 0, 0, 0), M);
		return M;
	}
	return i_sincos(A, FALSE);
}
//--------------------------------------------------------------------------
lista i_cos_inv(intervalo A, intervalo B) {
  return i_cos(B, A);
}
//--------------------------------------------------------------------------
lista i_arcc(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_arcc_inv(intervalo A, intervalo B) {
  return i_arcc(B, A);
}
//--------------------------------------------------------------------------
lista i_ccos(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_ccos_inv(intervalo A, intervalo B) {
  return i_ccos(B, A);
}
//--------------------------------------------------------------------------
// tangente
lista i_tan(intervalo A, intervalo B) {

	if (eh_vazio(A) || eh_vazio(B)) {
		lista M = cria_lista();
		enfileira(cria_intervalo(0, 0, 0, 0), M);
		return M;
	}

	double x = r_div(inf(A), M_PI); //TODO: e se essa divisao nao for definida?
	double y = r_div(sup(A), M_PI);

	double fa, fb;
	fa = r_tan(inf(A));
	fb = r_tan(sup(A));

	// ponto de referencia (em PIs) no grafico: dependendo do ponto de referencia
	// deve-se marcar o inicio como 1, 0.5 ou 1.5 PIs anteriores.
	double ref = floor(x) + 0.5;
	if (x > ref)
		ref += 1;

	intervalo X;
	if (y < ref)
		X = cria_intervalo(Te(A), Td(A), fa, fb);
	else
		X = cria_intervalo(0, 0, -INFINITY, INFINITY);
	
	lista M = cria_lista();
	enfileira(X, M);
	return M;
}
//--------------------------------------------------------------------------
lista i_tan_inv(intervalo A, intervalo B) {
  return i_tan(B, A);
}
//--------------------------------------------------------------------------
lista i_arct(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_arct_inv(intervalo A, intervalo B) {
  return i_arct(B, A);
}
//--------------------------------------------------------------------------
lista i_ctan(intervalo A, intervalo B) {
	return i_neutro(A, B);
}
//--------------------------------------------------------------------------
lista i_ctan_inv(intervalo A, intervalo B) {
  return i_ctan(B, A);
}
//--------------------------------------------------------------------------
