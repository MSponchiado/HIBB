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

#include "busca_local.h"
#include <stdio.h>
#include <stdlib.h>

unsigned long long int DEBUG_ERRO_INSTANCIACAO = 0;

//--------------------------------------------------------------------------

// Essa função vai considerar que N é um multi-intervalo de intervalos degenerados
// (ex: <[-2,-2], [2,2]>) e vai retornar qual o valor no multi-intervalo M mais
// próximo de N, isto é, mais próximo de qualquer um dos valores nos intervalos
// degenerados de N. A variável *distancia será setada com a distância entre esses
// valores.
double valor_mais_proximo_entre(Mintervalo M, Mintervalo N, double *distancia) {

  double value, distancia_tmp;

  *distancia = INFINITY;
  value = 0;

  for (no p = primeiro_no(N); p; p = proximo_no(p)) {
    intervalo X = objeto(p);
    if (!degenerado(X)) {
      // printf("ERRO: intervalo não degenerado na solução:");
      // imprime_intervalo(X);
      // printf("\n");
    //   exit(1);
    }
    double value_tmp = valor_mais_proximo_em(M, i_min(X), &distancia_tmp);
    if (distancia_tmp < *distancia) {
      *distancia = distancia_tmp;
      value = value_tmp;
    }
  }

  return value;
}

//--------------------------------------------------------------------------

// Devolve um valor do multi-intervalo M mais próximo do valor x e seta a
// variável *distancia com a distância entre esses valores. Logo, se x é um
// valor contido em M, o valor retornado será o próprio x, com distância 0.
double valor_mais_proximo_em(Mintervalo M, double x, double *distancia) {

  double value, dist;

  // verifica se x está ou não em em M,
  intervalo X = cria_intervalo(1, 1, x, x);
  int contido = FALSE;
  for (no p = primeiro_no(M); p && !contido; p = proximo_no(p))
    if (i_subset(X, objeto(p)))
      contido = TRUE;
  destroi_intervalo(X);

  // Se x está em M, então o erro é 0 e o valor procurado eh o proprio x
  if (contido) {
    *distancia = 0;
    value = x;
  }

  // Senão, é preciso calcular o erro
  else {
    *distancia = INFINITY;
    value = i_min(objeto(primeiro_no(M)));

    for (no p = primeiro_no(M); p; p = proximo_no(p)) {

      dist = r_abs(r_sub(i_min(objeto(p)), x));
      dist = normaliza_erro_RAC(dist, x);

      if (r_cmp(dist, "<", *distancia, &dist)) {
        *distancia = dist;
        value = i_min(objeto(p));
      }

      dist = r_abs(r_sub(i_max(objeto(p)), x));
      dist = normaliza_erro_RAC(dist, x);

      if (r_cmp(dist, "<", *distancia, &dist)) {
        *distancia = dist;
        value = i_max(objeto(p));
      }
    }
  }

  return value;
}

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
void calcula_melhor_rotacao(restricao R, Variavel X, Mintervalo *M, Mintervalo *N) {

 int rotacao = (X == cabeca(R) ? 0 : (X == cauda_E(R) ? 1 : 2));

  Variavel varX[3], varY[3], varZ[3];
  lista (*varOP[3]) (intervalo A, intervalo B);

  varX[0]  = R->X;
  varY[0]  = R->Y;
  varZ[0]  = R->Z;
  varOP[0] = R->f_op;

  varX[1]  = R->Y;
  varY[1]  = R->X;
  varZ[1]  = R->Z;
  varOP[1] = R->f_iop;

  varX[2]  = R->Z;
  varY[2]  = R->X;
  varZ[2]  = R->Y;
  varOP[2] = R->f_cop;

  double menor_comprimento = INFINITY;
  int menor_comprimento_rotacao = -1;

  for (int i = 0; i < 3; i++, rotacao = (rotacao+1)%3) {
    *N = Mop(multi_intervalo(varY[rotacao]), varOP[rotacao], multi_intervalo(varZ[rotacao]));
    double comprimento = M_comprimento(*N);
    if (comprimento < menor_comprimento) {
      menor_comprimento = comprimento;
      menor_comprimento_rotacao = rotacao;
    }
    destroi_Mintervalo(*N);
  }
  *M = multi_intervalo(varX[menor_comprimento_rotacao]);
  *N = Mop(multi_intervalo(varY[menor_comprimento_rotacao]), varOP[menor_comprimento_rotacao], multi_intervalo(varZ[menor_comprimento_rotacao]));
}

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
double infere_instanciacao(restricao R, Variavel X) {

    Mintervalo M, N;

    calcula_melhor_rotacao(R, X, &M, &N);

    double distancia;
    double value;

    //imprime_restricoes(R,0);
    //imprime_Mintervalo(M);
    //imprime_Mintervalo(N);
    value = valor_mais_proximo_entre(M, N, &distancia);
    destroi_Mintervalo(N);

    if (M == multi_intervalo(X)) {
        limpa_Mintervalo(multi_intervalo(X));
        enfileira(cria_intervalo(1, 1, value, value), multi_intervalo(X));
    }
    else
        M_real_valuation(multi_intervalo(X), 'b');

    return distancia;
}

//--------------------------------------------------------------------------
// Busca local do OGRe.
// Returna TRUE se há solução com a precisão pré-estabelecida.
// Vai empilhando em insatisfativeis os arcos que não são satisfeitas com a precisão dada.
// Aqui há 4 possibilidades:
// - o mínimo do intervalo objetivo é -inf: encerra a busca e retorna FALSE
// - alguma restrição de alguma árvore não foi satisfeita com a precisão dada (erros numéricos): encerra a busca e retorna FALSE
// - algum arco não foi satisfeito com a precisão dada: completa a busca, mas retorna FALSE e a lista de insatisfativeis preenchida
// - retorna TRUE: é uma solução satisfeita com a precisão dada (a lista de insatisfatíveis estará vazia)
int busca_local(epifita C, lista insatisfativeis) {

  int eh_solucao = TRUE;

  intervalo X;

  // atribui valor minimo ao intervalo objetivo
  Variavel OBJ = var_objetivo(C);
  double value = i_min(objeto(primeiro_no(multi_intervalo(OBJ))));

  limpa_Mintervalo(multi_intervalo(OBJ));
  enfileira(cria_intervalo(1, 1, value, value), multi_intervalo(OBJ));

  if (eh_minfinito(value))
    return FALSE;

  // vai percorrendo cada árvore, inferindo valores degenerados
  for (no a = primeiro_no(arvores(C)); a; a = proximo_no(a)) {

    // se tem arco, verifica erro relativo e infere valor ou abandona o processo
    restricao R = arco(objeto(a));

    if (R) {
      double precisao = infere_instanciacao(R, R->X);
      if (r_cmp(precisao, ">", ERRO_RAC, NULL)) {
        empilha(R, insatisfativeis);
        eh_solucao = FALSE;
      }
    }
    else
      // raiz da arvore a é uma restrição. Faz uma valoração real da cabeça dessa restrição
      M_real_valuation(multi_intervalo(cabeca(raiz(objeto(a)))), 'b');

    // para cada restricao da arvore
    for (no p = primeiro_no(odv(objeto(a))); p; p = proximo_no(p)) {
      R = objeto(p);

      int cauda_E_deg = M_degenerado(multi_intervalo(cauda_E(R)));
      int cauda_D_deg = M_degenerado(multi_intervalo(cauda_D(R)));

      // se caudas são não degeneradas, aplica consistencia
      if (!cauda_E_deg && !cauda_D_deg) {
        int conflito;
        local_consistency(R, &conflito);
        if (conflito) {
          DEBUG_ERRO_INSTANCIACAO++;
          return FALSE;
        }
      }

      if (cauda_E_deg && cauda_D_deg) {
          X = objeto(primeiro_no(multi_intervalo(cauda_E(R))));
          X->b = X->a;
          X = objeto(primeiro_no(multi_intervalo(cauda_D(R))));
          X->b = X->a;
      }
      else if (cauda_E_deg && !cauda_D_deg) {
          X = objeto(primeiro_no(multi_intervalo(cauda_E(R))));
          X->b = X->a;
          double precisao = infere_instanciacao(R, R->Z);
          if (r_cmp(precisao, ">", ERRO_RAC, NULL)) {
            DEBUG_ERRO_INSTANCIACAO++;
            return FALSE;
          }
      }
      else if (!cauda_E_deg && cauda_D_deg) {
          X = objeto(primeiro_no(multi_intervalo(cauda_D(R))));
          X->b = X->a;
          double precisao = infere_instanciacao(R, R->Y);
          if (r_cmp(precisao, ">", ERRO_RAC, NULL)) {
            DEBUG_ERRO_INSTANCIACAO++;
            return FALSE;
          }
      }
      else if (!cauda_E_deg && !cauda_D_deg) {
          M_real_valuation(multi_intervalo(cauda_D(R)), 'b');
          double precisao = infere_instanciacao(R, R->Y);
          if (r_cmp(precisao, ">", ERRO_RAC, NULL)) {
            DEBUG_ERRO_INSTANCIACAO++;
            return FALSE;
          }
      }
    }
  }

  return eh_solucao;
}
