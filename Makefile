# CFLAGS  = -g -O3 -std=c99 -W -Wall -Wextra -Isolvers -Wno-unused-variable
CFLAGS  = -g -O3 -std=c99 -W -Wall -Wextra -Isolvers

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : rotaciona executa

rotaciona : OGRe/rotacionador.o OGRe/rotacionador_lib.o OGRe/intervalo.o OGRe/lista.o OGRe/operacoes.o OGRe/real.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

executa : OGRe/ogre-de.o OGRe/dot.o OGRe/heuristica.o OGRe/branch-bound.o OGRe/epifita.o \
	OGRe/arvore_restricoes.o OGRe/variavel.o OGRe/multi-intervalo.o OGRe/intervalo.o OGRe/lista.o \
	OGRe/real.o OGRe/busca_local.o OGRe/operacoes.o bbde/bbde_buscalocal.o bbde/bbde_adaptParam.o \
	bbde/bbde_alocDef.o bbde/bbde_operacoes.o bbde/bbde_problema.o bbde/bbde_uteis.o \
	main.o adaptParam.o alocDef.o contratores.o heap.o problema.o rand.o uteis.o instancia.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl -lm

#------------------------------------------------------------------------------
# para gerar as regras para os .o
# for i in OGRe/*.c bbde/*.c *.c; do cc -MM $i; done > regras.mk
include regras.mk

#------------------------------------------------------------------------------
clean :
	$(RM) rotaciona executa OGRe/*.o bbde/*.o *.o *.out *.dot
