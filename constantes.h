#ifndef _CONSTANTES_H_
#define _CONSTANTES_H_

#include <float.h>

#define M_PI 3.14159265358979323846
#define MAXPOP 500          // utilizado somente no OGRe
#define INFINITODE 1.0e+15
#define TOLERANCIA 1.0e-10
#define CALCDIMENSOES 0

// contratores
#define APLICA_CGAC 1
#define APLICA_CESTOC 0
#define APLICA_CBORDA 0

#define PODA_BSFUNOBJ 1     // soluções factíveis podam a borda superior da F.O.
#define LOG_GAC 1           // registra dados da aplicação de GAC
#define FATOR_PARTIC 0.0    // insere borda na contração estocástica

#define MARGEMPONTO 0.10     // margem em torno do bestpoint para criar a caixa

// epsilon level
#define APLICA_EPSLVL 0     // epsilon tolerância ao comparar duas soluções
#define APLICA_EPLVLMIN 0   // em caso de epsilon inicial 0, usa um valor mínimo
#define EPSLVLMIN 1.0e-03   // valor mínimo a ser usado em caso de epsilon inicial 0

// abandona a caixa quando sua maior dimensão atinge um tamanho mínimo DIM_MIN
#define ABANDONA_CAIXA 1
#define DIM_MIN 0.001

// aborta execução do BBDE no caso de MEM_SUCC_BBDE gerações sem melhoria
#define ABORTA_BBDE 0
#define MEM_SUCC_BBDE 50

// multiplicador do número de buscas em paralelo
#define MULTNBUSCAS 0.5

// prioridade do heap
#define PRIORITY_BY_STAGNATION 0

#endif
