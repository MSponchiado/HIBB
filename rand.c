#define _XOPEN_SOURCE
#include <stdlib.h>
#include "rand.h"

void random_init(long int seed) {
  srand48(seed);
}

double gera_random() {
  return drand48();
}

double gauss(double mu, double sigma) {
  double result = mu + sigma * sqrt(-2.0 * log(gera_random())) * sin(2.0 * M_PI * gera_random());
  return result;
}

double cauchy_g(double mu, double gamma) {
  double result =  mu + gamma * tan(M_PI*(gera_random() - 0.5));
  return result;
}
