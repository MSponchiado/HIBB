#include "real.h"
#include "branch-bound.h"
#include "../inter.h"
#include "ogre-de.h"
#include "dot.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fenv.h>
#include <time.h>

int ENVOLTORIA_INDE = FALSE;

/******************************************************************************
 Cria decomposição epifita com n_vars variáveis e aplica a consistência de arco
 (reaproveitei o código do main do OGRe)

 Entrada: endereço da variável n_vars. Lê arquivo da instância do problema.
 Saída: n_vars com o número de variáveis da decomposição e retorna a decomposição epífita
*******************************************************************************/
epifita epifita_init(int *n_vars, int *omega, int *restr_tern) {

  // srand(time(NULL));

  /*---------Parâmetros OGRe-------------------------------------*/
  EPSILON =  TOLERANCIA;
  EPSILON_REL = TOLERANCIA;
  FULL_CONSISTENCY = TRUE;
  // MAX_STEPS_CONSISTENCY = 1000;
  TIPO_ERRO_RAC = ERRO_ABS_REL;
  // TIPO_ERRO_RAC = ERRO_ABSOLUTO;
  DELTA = 0.01;
  ERRO_RAC = 0.01;
  ESCOLHA_ALEATORIA_INDE = FALSE;
  ENVOLTORIA_INDE = TRUE;

  int erro = 0;

  // lista de variáveis
  lista l = cria_lista();
  no p, n;

  Variavel M, N;

  // nome das variáveis
  char X[32], Y[32], Z[32];
  // flag
  char flag;
  // caracteres de operação, igualdade, parenteses ou colchetes
  // (topologia dos limitantes - aberto ou fechado)
  char oper[8], eq, T_e, T_d;

  // limitantes dos intervalos
  double a, b;

  // tipo de uma restrição (1:arco; 2:arvore; 3:arvore desconexa)
  int tipo;

  // Arquivo começa com as linhas da forma:
  //
  // v nomevar [ -32.567 , inf ]
  //
  // onde v é uma flag indicando descrição de variável,
  // nomevar é o nome da variável,
  // '[' também pode ser '(', assim como ']' pode ser ')',
  // e os limitantes podem ser floats (positivos e negativos),
  // -inf ou inf
  //
  // vai lendo as variáveis até a flag ser diferente de v,
  // sempre enfileirando as Variaveis criados em l
  if (!scanf("%c", &flag)) erro = 1;
  *n_vars = 0;

  while (flag == 'v') {
    if (!scanf("%s %c %lf %c %lf %c %c", X, &T_e, &a, &eq, &b, &T_d, &flag)) erro = 1;
    if (eh_minfinito(a)) T_e = '(';
    if (eh_infinito(b))  T_d = ')';
    intervalo I = cria_intervalo( (T_e == '[')?(1):(0), (T_d == ']')?(1):(0), a, b);
    M = cria_Variavel(X, I, *n_vars);
    *n_vars = *n_vars + 1;
    enfileira(M, l);
  }

  if (erro) {
    printf("Erro na leitura das variaveis da decomposicao Epifita.\n");
    exit(1);
  }

  // começo com uma árvore vazia na Árvore Epífita
  epifita C = cria_epifita();
  arvore A = cria_arvore();
  enfileira(A, arvores(C));

  // variável auxiliar que será incluída em árvores
  // que tenham apenas arco (o que foge da definição).
  // Assim, vamos criar pelo menos uma restrição fantasma
  // para cada. Variáveis auxiliares chamar-se-ão #1, #2, etc.
  // Obs: um arco é uma restrição do conjunto Omega da Árvore Epífita
  int var_aux_cont = 1;
  char var_aux[32];
  var_aux[0] = '#';
  var_aux[1] = 'a';

  // Arquivo tem que ter pelo menos uma restrição, da forma:
  //
  // c t X = Y oper Z
  //
  // onde c é a flag de restrição, t \in {1,2,3} é o tipo, X, Y e Z são nomes de
  // variáveis, = é o símbolo de igualdade e oper é o símbolo
  // de operação (caractere).
  if (!scanf("%d %s %c %s %s %s %c", &tipo, X, &eq, Y, oper, Z, &flag)) erro = 1;

  if (erro) {
    printf("Erro na leitura das restricoes da decomposicao Epifita.\n");
    exit(1);
  }

  // busca as variáveis de nomes X, Y e Z na lista l e cria
  // uma restrição R com elas. Em seguida, adiciona R na
  // primeira árvore criada, pois R é raiz com intervalo objetivo.
  restricao R = cria_restricao(
                     objeto(busca(X, l, compara_Variavel)),
                     objeto(busca(Y, l, compara_Variavel)),
                     objeto(busca(Z, l, compara_Variavel)),
                     opcode(oper));
  adiciona_restricao(R, A, NULL);

  *restr_tern = 0;
  *omega = 0;
  // E continua com esse processo para todas as restrições
  while (flag == 'c') {

      (*restr_tern)++;

      if (!scanf("%d %s %c %s %s %s %c", &tipo, X, &eq, Y, oper, Z, &flag)) erro = 1;

      if (erro) {
        printf("Erro na leitura das restricoes da decomposicao Epifita.\n");
        exit(1);
      }

      R = cria_restricao(
                         objeto(busca(X, l, compara_Variavel)),
                         objeto(busca(Y, l, compara_Variavel)),
                         objeto(busca(Z, l, compara_Variavel)),
                         opcode(oper));

    // com a pequena diferença de que, a cada passo, não sabemos
    // em qual árvore adicionar R. (depende do seu tipo)

    // restricao eh um arco (obs.: eh garantido que o arco vem sempre
    // antes da sua arvore, pois do contrário não poderia ter uma cabeça)
    if (tipo == 1) {

      (*omega)++;

      // cria uma nova árvore com R como arco
      A = cria_arvore();
      enfileira(A, arvores(C));
      adiciona_arco(R, A);

      // Já garante que a nova árvore não terá apenas arco,
      // criando uma restrição fantasma.
      sprintf(&(var_aux[2]), "%d", var_aux_cont++);
      M = cria_Variavel(var_aux, cria_intervalo(0, 0, -INFINITY, INFINITY), *n_vars);
      *n_vars = *n_vars + 1;
      enfileira(M, l);

      sprintf(&(var_aux[2]), "%d", var_aux_cont++);
      N = cria_Variavel(var_aux, cria_intervalo(0, 0, -INFINITY, INFINITY), *n_vars);
      *n_vars = *n_vars + 1;
      enfileira(N, l);

      R = cria_restricao(cabeca(R), M, N, opcode("ADD"));
      adiciona_restricao(R, A, NULL);
    }

    // restricao vai em alguma arvore já existente
    else if (tipo == 2) {

      // procura a arvore
      n = NULL;
      p = primeiro_no(arvores(C));
      int i = 1;
      while (p && !(n = busca(R, variaveis(objeto(p)), compara_cabeca_restricao))) {
        p = proximo_no(p);
        i++;
      }
      if (!p) printf("ERRO CONSTRUINDO A ARVORE EPIFITA\n");

      adiciona_restricao(R, objeto(p), n);
    }

    // restricao vai criar uma nova arvore, desconexa das demais
    else {
      // cria uma nova árvore com R como restricao
      A = cria_arvore();
      enfileira(A, arvores(C));
      adiciona_restricao(R, A, NULL);
    }
  }

  calcula_complete_odv(C);
  seta_variavel_objetivo(C);
  // gera_dot(C, NULL);

  // Como variáveis auxiliares foram criadas, mas depois excluídas, e podem existir variáveis
  // que não entraram na epífita, é preciso recalcular a quantidade de variáveis, juntamente com os ids.
  *n_vars = 0;
  for (p = primeiro_no(l); p; p = proximo_no(p)) {
    M = objeto(p);
    if (primeiro_no(restricoes(M)) == NULL) {
      if (M->nome[0] == '$') {
        printf("ERROR: variável original %s não usada\n", nome(M));
        exit(1);
      }
      // else
        // printf("WARNING: variável %s removida\n", nome(M));
    }
    else {
      M->index = *n_vars;
      *n_vars = *n_vars + 1;
    }
  }

  int conflitante;
  if (FULL_CONSISTENCY)
    conflitante = global_consistency(C, NULL);
  else
    conflitante = directional_consistency(C, NULL, 1);
  if (conflitante) return NULL;

  return C;
}

/******************************************************************************
 Cria a população inicial usando o B&B.
 Entrada: decomposição epífita C preenchida, MAXSIZE_C (tamanho máximo da população) e MAX_PROFUNDIDADE (nível máximo do B&B)
 Saída: matriz de intervalos "retorno" (população) e o seu número de indivíduos "retorno_size".
 Retorna 0 em caso de sucesso e -1 em caso de erro (isto é, erro na função constroi_populacao()).
 *******************************************************************************/
int vec_inter_init(epifita C, t_indice *retorno, int *retorno_size, int MAXSIZE_C, int MAX_PROFUNDIDADE) {

  double tempo_de_execucao;

  lista nodos = branch_and_bound(C, MAX_PROFUNDIDADE, &tempo_de_execucao);

  for (no p = primeiro_no(nodos); p; p = proximo_no(p)) {
    aplica(C, valoracao(objeto(p)));
    int erro;
    if (ENVOLTORIA_INDE) {
      constroi_envoltoria_individuo(C, retorno, retorno_size);
      erro = (*retorno_size >= MAXSIZE_C);
    }
    else {
      erro = constroi_populacao(C, retorno, retorno_size, MAXSIZE_C);
    }
    if (erro) {
      destroi_lista(nodos, destroi_ramo);
      return -1;
    }
  }
  destroi_lista(nodos, destroi_ramo);
  return 0;
}

/******************************************************************************
  Essa função recebe a decomposição Epífita C preenchida e preenche o vetor_arcos
  com os índices das variáveis que compõem os arcos da decomposição. Este é um
  vetor do tipo t_restr, que é uma estrutura específica do InDE para armazenar os
  índices. Retorna também a variável tam_vetor_arcos com o tamanho do vetor preenchido.
 *******************************************************************************/
void vec_restr_init(epifita C, t_restr *vetor_arcos, int *tam_vetor_arcos) {

  int i = 0;

  for (no n = primeiro_no(arvores(C)); n; n = proximo_no(n)) {
    restricao R = arco(objeto(n));
    if (R) {
      vetor_arcos[i].idc = indexV(cabeca(R));
      vetor_arcos[i].ide = indexV(cauda_E(R));
      vetor_arcos[i].idd = indexV(cauda_D(R));
      i++;
    }
  }
  *tam_vetor_arcos = i;
}

/******************************************************************************
 Acrescenta na matriz da populacao um individuo gerado pela envolvoria dos multi-intervalos.

 Entrada: Epifita C, matriz "retorno" já alocada de MAXPOP linhas e MAXDIM colunas e o
 tamanho da populacao (retorno_size).
 Saída: matriz preenchida e o número de linhas ocupadas (retorno_size).
 *******************************************************************************/
void constroi_envoltoria_individuo(epifita C, t_indice *retorno, int *retorno_size) {

  // para cada variável da Epífita
  for (no q = primeiro_no(arvores(C)); q; q = proximo_no(q))
    for (no p = primeiro_no(variaveis(objeto(q))); p; p = proximo_no(p)) {
      Variavel X = objeto(p);
      retorno[*retorno_size].ind[indexV(X)].bi = i_min(objeto(primeiro_no(multi_intervalo(X))));
      retorno[*retorno_size].ind[indexV(X)].bs = i_max(objeto(ultimo_no(multi_intervalo(X))));
    }
  *retorno_size = (*retorno_size) + 1;
}

/******************************************************************************
 Constroi matriz com populacao quebrando os multi-intervalos

 Entrada: Epifita C, matriz "retorno" já alocada de MAXPOP linhas e MAXDIM colunas e o
 tamanho da populacao (retorno_size). MAXSIZE_C é o tamanho máximo permitido da população.
 Saída: matriz preenchida e o número de linhas ocupadas (retorno_size). Devolve 0 em
 caso de sucesso e -1 em caso de falha (TODO: qual falha?)
 *******************************************************************************/
int constroi_populacao(epifita C, t_indice *retorno, int *retorno_size, int MAXSIZE_C) {

  // printf("constroi populacao\n");

  int offset = *retorno_size;
  int linha = offset;

  // TODO: aqui tem um bug. Pode ser que essa função seja chamada com offset >= MAXSIZE_C.
  // Nesse caso, o limite_atingido não será setado (acho que deveria ter uma outra flag 'limite_extrapolado'.
  // O que fazer? Descarta os ramos do B&B? (É isso o que esse if está fazendo, por enquanto).
  if (offset >= MAXSIZE_C) return -1;

  int limite_atingido = (offset == MAXSIZE_C-1);

  // para cada variável da Epífita
  for (no q = primeiro_no(arvores(C)); q; q = proximo_no(q))
    for (no p = primeiro_no(variaveis(objeto(q))); p; p = proximo_no(p)) {

      Variavel X = objeto(p);

      // se X não possui arcos então não pertence a Omega: cria envoltória
      // e copia para todas as linhas válidas da matriz, na coluna referente a essa variável
      // A mesma coisa se atingiu o limite de MAXSIZE_C, pois dai não é possível mais ramificar
      if (!primeiro_no((restarcos(X))) || limite_atingido) {
        for (int i = offset; i <= linha; i++) {
          retorno[i].ind[indexV(X)].bi = i_min(objeto(primeiro_no(multi_intervalo(X))));
          retorno[i].ind[indexV(X)].bs = i_max(objeto(ultimo_no(multi_intervalo(X))));
        }
      }

      // senão, então é preciso ramificar os multi-intervalos
      else {

        // pega o primeiro intervalo dessa variável
        no n = primeiro_no(multi_intervalo(X));
        intervalo I = objeto(n);
        // e copia esse intervalo para todas as linhas válidas da matriz, na coluna referente a essa variável
        for (int i = offset; i <= linha; i++) {
          retorno[i].ind[indexV(X)].bi = i_min(I);
          retorno[i].ind[indexV(X)].bs = i_max(I);
        }

        // agora é preciso verificar se há mais intervalos na variável (multi-intervalo)
        int j = linha;
        // para cada um desses outros intervalos
        for (n = proximo_no(n); n;) {
          I = objeto(n);

          // verifica se vai atingir o limite de MAXPOP
          int tamanho = j - offset + 1;
          if (offset + tamanho * 4 > MAXSIZE_C) {
            limite_atingido = TRUE;

            // printf("LIMITE: %d + %d > %d\n", offset, tamanho * 4, MAXSIZE_C);

            // atingiu o limite de ramificação. Para o restante desse multi-intervalo (que vai de I até ultimo_no(multi_intervalo(X)))
            // cria apenas um último intervalo usando a envoltória (por isso na condição faz * 4 ao invés de * 2

            I = cria_intervalo(1, 1, i_min(I), i_max(objeto(ultimo_no(multi_intervalo(X)))));
            n = NULL;
          }
          else {
            n = proximo_no(n);
          }

          // e para cada linha já preenchida na matriz
          for (int i = offset; i <= linha; i++) {
            // j é a próxima linha a ser preenchida
            j++;
            // verifica se não estourou a memória alocada
            if (j >= MAXSIZE_C) {
              printf("ERRO: Atingiu MAXSIZE_C! Limite = %d, offset = %d, tamanho = %d, total = %d\n", limite_atingido, offset, (j-1) - offset + 1, offset + ((j-1) - offset + 1) * 4);
              *retorno_size = MAXSIZE_C;
              return -1;
            }

            // copia para a linha j o intervalo corrente
            retorno[j].ind[indexV(X)].bi = i_min(I);
            retorno[j].ind[indexV(X)].bs = i_max(I);

            // e para todas as variáveis já processadas, copia para a linha j o respectivo intervalo
            // na linha i
            for (no q1 = primeiro_no(arvores(C)); q1;) {
              for (no p1 = primeiro_no(variaveis(objeto(q1))); p1;) {
                if (objeto(p1) != objeto(p)) {
                  Variavel Y = objeto(p1);
                  retorno[j].ind[indexV(Y)].bi = retorno[i].ind[indexV(Y)].bi;
                  retorno[j].ind[indexV(Y)].bs = retorno[i].ind[indexV(Y)].bs;

                  p1 = proximo_no(p1);
                }
                else {
                  p1 = q1 = NULL;
                }
              }
              if (q1) q1 = proximo_no(q1);
            }
          }
        }
        linha = j;
      }
    }

  *retorno_size = linha+1;
  return 0;
}

/******************************************************************************
 Altera todos os multi-intervalos da epifita C para os intervalos dados no
 vetor_de_intervalos.
*******************************************************************************/
void reseta_epifita(t_inter *vetor_de_intervalos, epifita C) {

  for (no q = primeiro_no(arvores(C)); q; q = proximo_no(q))
    for (no p = primeiro_no(variaveis(objeto(q))); p; p = proximo_no(p)) {
      Variavel X = objeto(p);
      double bi = vetor_de_intervalos[indexV(X)].bi;
      double bs = vetor_de_intervalos[indexV(X)].bs;
      limpa_Mintervalo(multi_intervalo(X));
      enfileira(cria_intervalo(1, 1, bi, bs), multi_intervalo(X));
    }
}

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
//                  t_indice *retorno, int *retorno_size, int MAXSIZE_C, int RETORNA_CONSISTENTE) {

int evaluateOGRe(epifita C, t_inter *vetor_de_intervalos, t_indice *retorno,
  int *retorno_size) {

 // double solucao, soma_epsilon;

  // copia os intervalos do vetor_de_intervalos para a arvore epifita C
  reseta_epifita(vetor_de_intervalos, C);

  // aplica a consistência de arco em C
  // (se for conflitante, não altera o vetor_de_intervalos)
  int conflito_consistencia;
  if (FULL_CONSISTENCY) {
    // lista de variaveis alteradas pelo DE
    //lista vars = cria_lista();
    //for (no q = primeiro_no(arvores(C)); q; q = proximo_no(q)) {
    //  restricao R = arco(objeto(q));
    //  if (R) {
    //    enfileira(cauda_E(R), vars);
    //    enfileira(cauda_D(R), vars);
    //  }
    //}
    //conflito_consistencia = global_consistency(C, vars);
    conflito_consistencia = global_consistency(C, NULL);
  }
  else
    conflito_consistencia = directional_consistency(C, NULL, 1);

  if (conflito_consistencia) return 1;

  // se deve retornar os intervalos consistentes, chama a função constroi_populacao
  // para quebrar multi-intervalos e construir a matriz de intervalos (população)
  // chamada de "retorno", cujo tamanho é "retorno_size".
  if (*retorno_size < (MAXPOP - 5)) {
    if (ENVOLTORIA_INDE) {
      constroi_envoltoria_individuo(C, retorno, retorno_size);
    }
    else {
      int erro = constroi_populacao(C, retorno, retorno_size, MAXPOP - 1);
      if (erro) return -1;
    }
  }

  // busca solucao
  //int conflito_busca = busca_local_InDE(C, &solucao, &soma_epsilon);

  // if (conflito_busca) return 2;

  return 0;
}

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
int busca_local_InDE(epifita C, double *solucao, double *soma_epsilon) {

  //lista labels = labels_dot(C);

  int tam_vetor = 0;

  intervalo X;

  // atribui valor minimo ao intervalo objetivo
  Variavel OBJ = var_objetivo(C);
  double value = i_min(objeto(primeiro_no(multi_intervalo(OBJ))));

  if (eh_minfinito(value)) {
    //return TRUE;              // TODO: Essa valoração não garante ser a ótima deste ramo do branch and bound
                                //       No InDE, isso não é tão relevante.
    M_real_valuation(multi_intervalo(OBJ), 'b');
    *solucao = inf(objeto(primeiro_no(multi_intervalo(OBJ))));
  }
  else {
    *solucao = value;

    limpa_Mintervalo(multi_intervalo(OBJ));
    enfileira(cria_intervalo(1, 1, value, value), multi_intervalo(OBJ));
  }

  *soma_epsilon = 0;

  // vai percorrendo cada árvore, inferindo valores degenerados
  for (no a = primeiro_no(arvores(C)); a; a = proximo_no(a)) {

    // se tem arco, verifica erro relativo e infere valor ou abandona o processo
    // no InDE nao abandona o processo, apenas acrescenta no epsilon e continua a busca
    restricao R = arco(objeto(a));

    if (R) {
      double precisao = infere_instanciacao(R, R->X);
      // vetor_epsilon[tam_vetor] = precisao;
      *soma_epsilon = *soma_epsilon + precisao;
      tam_vetor++;
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
        int confl;

        local_consistency(R, &confl);
        if (confl) {
          DEBUG_ERRO_INSTANCIACAO++;
          return TRUE;
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
          infere_instanciacao(R, R->Z);
          // double precisao = infere_instanciacao(R, R->Z);
          // if (r_cmp(precisao, ">", ERRO_RAC, NULL)) {
          //   DEBUG_ERRO_INSTANCIACAO++;
          //   return TRUE;
          // }
      }
      else if (!cauda_E_deg && cauda_D_deg) {
          X = objeto(primeiro_no(multi_intervalo(cauda_D(R))));
          X->b = X->a;
          infere_instanciacao(R, R->Y);
          // double precisao = infere_instanciacao(R, R->Y);
          // if (r_cmp(precisao, ">", ERRO_RAC, NULL)) {
          //   DEBUG_ERRO_INSTANCIACAO++;
          //   return TRUE;
          // }
      }
      else if (!cauda_E_deg && !cauda_D_deg) {
          M_real_valuation(multi_intervalo(cauda_D(R)), 'b');
          infere_instanciacao(R, R->Y);
          // double precisao = infere_instanciacao(R, R->Y);
          // if (r_cmp(precisao, ">", ERRO_RAC, NULL)) {
          //   DEBUG_ERRO_INSTANCIACAO++;
          //   return TRUE;
          // }
      }
    }
  }

  //update_labels_dot(C, labels);
  //gera_dot(C, labels);
  //exit(1);

  return FALSE;
}
