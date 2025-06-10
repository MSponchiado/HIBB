#ifndef _OGRE_DE_
#define _OGRE_DE_

#include "../constantes.h"
#include "../inter.h"
#include "real.h"
#include "branch-bound.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fenv.h>

extern int ENVOLTORIA_INDE; // Devolve os intervalos consistentes como conjuntos de caixas (FALSE)
                            // ou uma única caixa com a envoltória da solução (TRUE)

/******************************************************************************
 Cria decomposição epifita com n_vars variáveis e aplica a consistência de arco
 (reaproveitei o código do main do OGRe)

 Entrada: endereço da variável n_vars. Lê arquivo da instância do problema.
 Saída: n_vars com o número de variáveis da decomposição e retorna a decomposição epífita
*******************************************************************************/
epifita epifita_init(int *n_vars, int *omega, int *restr_tern);

/******************************************************************************
 Cria a população inicial usando o B&B.
 Entrada: decomposição epífita C preenchida, MAXSIZE_C (tamanho máximo da população) e MAX_PROFUNDIDADE (nível máximo do B&B)
 Saída: matriz de intervalos "retorno" (população) e o seu número de indivíduos "retorno_size".
 Retorna 0 em caso de sucesso e -1 em caso de erro (isto é, erro na função constroi_populacao()).
 *******************************************************************************/
int vec_inter_init(epifita C, t_indice *retorno, int *retorno_size, int MAXSIZE_C, int MAX_PROFUNDIDADE);

/******************************************************************************
  Essa função recebe a decomposição Epífita C preenchida e preenche o vetor_arcos
  com os índices das variáveis que compõem os arcos da decomposição. Este é um
  vetor do tipo t_restr, que é uma estrutura específica do InDE para armazenar os
  índices. Retorna também a variável tam_vetor_arcos com o tamanho do vetor preenchido.
 *******************************************************************************/
void vec_restr_init(epifita C, t_restr *vetor_arcos, int *tam_vetor_arcos);

/******************************************************************************
 Constroi matriz com populacao quebrando os multi-intervalos

 Entrada: Epifita C, matriz "retorno" já alocada de MAXPOP linhas e MAXDIM colunas e o
 tamanho da populacao (retorno_size). MAXSIZE_C é o tamanho máximo permitido da população.
 Saída: matriz preenchida e o número de linhas ocupadas (retorno_size). Devolve 0 em
 caso de sucesso e -1 em caso de falha (TODO: qual falha?)
 *******************************************************************************/
void constroi_envoltoria_individuo(epifita C, t_indice *retorno, int *retorno_size);

/******************************************************************************
 Constroi matriz com populacao quebrando os multi-intervalos

 Entrada: Epifita C, matriz "retorno" já alocada de MAXPOP linhas e MAXDIM colunas e o
 tamanho da populacao (retorno_size). MAXSIZE_C é o tamanho máximo permitido da população.
 Saída: matriz preenchida e o número de linhas ocupadas (retorno_size). Devolve 0 em
 caso de sucesso e -1 em caso de falha (TODO: qual falha?)
 *******************************************************************************/
int constroi_populacao(epifita C, t_indice *retorno, int *retorno_size, int MAXSIZE_C);

/******************************************************************************
 Altera todos os multi-intervalos da epifita C para os intervalos dados no
 vetor_de_intervalos.
*******************************************************************************/
void reseta_epifita(t_inter *vetor_de_intervalos, epifita C);

/******************************************************************************
 Avalia a árvore epífita com o vetor de contexto.

 Recebe como entrada a decomposição epífita C.
 Recebe também um vetor_de_intervalos correspondente a todas as variáveis da decomposição.
 Executa os seguintes passos:
 1- atualiza os intervalos da decomposição usando o vetor_de_intervalos
 2- aplica a consistência direcionada ou global
 3- constrói uma população de retorno, se for o caso
 4- executa uma busca local para obter um valor de solucao, soma_epsilon e vetor_epsilon.

 Recebe também duas flags: MAXSIZE_C e RETORNA_CONSISTENTE (se for TRUE deve retornar
 os intervalos consistentes, construindo a populacao na matriz "retorno")

 Retornos:
 *solucao       => solucao ótima dessa caixa (relaxada)
 *soma_epsilon  => soma dos erros dos arcos da decomposição epífita
 *vetor_epsilon => vetor com os erros dos arcos da decomposição epífita
 **retorno      => matriz de intervalos: cada linha é um indivíduo
 *retorno_size  => número de linhas da matriz
 return 1       => houve conflito na consistência
 return 2       => houve conflito na busca local
 return -1      => houve erro ao construir a população
 return 0       => sucesso
********************************************************************************/
// int evaluateOGRe(epifita C, t_inter *vetor_de_intervalos, double *solucao, double *soma_epsilon,
//                  t_indice *retorno, int *retorno_size, int MAXSIZE_C, int RETORNA_CONSISTENTE);
int evaluateOGRe(epifita C, t_inter *vetor_de_intervalos, t_indice *retorno, int *retorno_size);

//--------------------------------------------------------------------------
// Versão da busca local utilizada no InDE (adaptada de busca_local() do OGRe).
// Recebe a epifita C e devolve a solucao minima desse ramo juntamente com
// um vetor_epsilon com os erros de cada restricao do conjunto
// Omega e a soma_epsilon desses erros.
// Principais diferenças com o busca_local():
// - faz sempre a busca até o final, independente do erro nos arcos;
// - armazena todos os erros nos arcos e devolve no vetor vetor_epsilon
// - NÃO VERIFICA ERROS NUMÉRICOS (isso é certo?)
// - retorna TRUE se houve conflito ou valor objetivo é -inf
// - retorna FALSE se não houve conflito
//--------------------------------------------------------------------------
int busca_local_InDE(epifita C, double *solucao, double *soma_epsilon);

#endif
