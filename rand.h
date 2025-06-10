#ifndef _RAND_H_
#define _RAND_H_

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <math.h>
#include "OGRe/real.h"
#include "OGRe/ogre-de.h"
#include "inter.h"
#include "alocDef.h"
#include "heap.h"
#include "problema.h"
#include "constantes.h"

void random_init(long int);

double gera_random();

double gauss(double, double);

double cauchy_g(double, double);

#endif
