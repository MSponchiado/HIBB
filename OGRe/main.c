#include "ogre-de.h"

int main(void) {

  // constroi epifita
  int n_vars;
  epifita C = epifita_init(&n_vars);

  imprime_variaveis(C);

  // vou testar com 30 individuos
  int NP = 30;

  // aloca matriz da populacao
  t_inter **pop1 = (t_inter **) malloc (MAXPOP * sizeof(t_inter *));
  for (int i = 0; i < MAXPOP; i++)
    pop1[i] = (t_inter *) malloc (n_vars * sizeof(t_inter));

  t_indice *ipop1 = (t_indice *) malloc (MAXPOP * sizeof(t_indice));
  for (int i = 0; i < MAXPOP; i++)
    (*ipop1)[i].ind = (*pop1)[i];

  // cria individuo inicial pop1[0]
  for (no n = primeiro_no(arvores(C)); n; n = proximo_no(n))
    for (no p = primeiro_no(variaveis(objeto(n))); p; p = proximo_no(p)) {
      Variavel X = objeto(p);
      double bi = i_min(objeto(primeiro_no(multi_intervalo(X))));
      double bs = i_max(objeto(ultimo_no(multi_intervalo(X))));
      ipop1[0].ind[indexV(X)].bi = bi;
      ipop1[0].ind[indexV(X)].bs = bs;
    }

  // cria um vetor apenas com as variaveis originais do problema:
  Variavel originais[1024];
  int tam_originais = 0;
  for (no n = primeiro_no(arvores(C)); n; n = proximo_no(n))
    for (no p = primeiro_no(variaveis(objeto(n))); p; p = proximo_no(p)) {
      char *name = nome(objeto(p));
      if (name[0] == '$') {
        originais[tam_originais] = objeto(p);
        tam_originais++;
      }
    }

  // cria populacao usando o B&B
  int popBeB, profundidade;
  popBeB = profundidade = 1;
  while(popBeB < NP) {
    vec_inter_init(C, pop1, &popBeB, NP, profundidade);
    profundidade++;
    reseta_epifita(pop1[0], C);
  }

  // conta a quantidade de arcos
  int tam_vetor_epsilon = 0;
  for (no n = primeiro_no(arvores(C)); n; n = proximo_no(n))
    if (arco(objeto(n)))
      tam_vetor_epsilon++;

  // declara variaveis para calculo do fitness
  double solucao;
  double soma_epsilon;
  double vetor_epsilon[tam_vetor_epsilon];
  // avalia cada individuo da populacao
  for (int i = 0; i < popBeB; i++) {
    evaluateOGRe(C, pop1[i], &solucao, &soma_epsilon, vetor_epsilon, ipop1, &popBeB, NP, FALSE);
    printf("%d: %lf %lf\n", i, solucao, soma_epsilon);
    for (int j = 0; j < tam_originais; j++)
      printf("%s = %f; ", nome(originais[j]), inf(objeto(primeiro_no(multi_intervalo(originais[j])))));
    double x1 = inf(objeto(primeiro_no(multi_intervalo(originais[3]))));
    double x2 = inf(objeto(primeiro_no(multi_intervalo(originais[2]))));
    double x3 = inf(objeto(primeiro_no(multi_intervalo(originais[1]))));
    double x4 = inf(objeto(primeiro_no(multi_intervalo(originais[0]))));
    double obj = x1 + x2 + x3 + x4;
    double e1 = 4/x1 + 2.25/x2 + 1/x3 + 0.25/x4;
    double e2 = 0.16/x1 + 0.36/x2 + 0.64/x3 + 0.64/x4;
    printf("\nobj = %f\ne1: %f\ne2: %f\n", obj, e1 > 0.0401 ? e1 - 0.0401 : 0, e2 > 0.010085 ? e2 - 0.010085 : 0);
    printf("\n-----\n");
  }
  return 0;
}
