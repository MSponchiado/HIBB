arvore_restricoes.o: OGRe/arvore_restricoes.c OGRe/arvore_restricoes.h \
 OGRe/variavel.h OGRe/multi-intervalo.h OGRe/intervalo.h OGRe/lista.h \
 OGRe/real.h OGRe/../constantes.h OGRe/operacoes.h
branch-bound.o: OGRe/branch-bound.c OGRe/branch-bound.h OGRe/epifita.h \
 OGRe/arvore_restricoes.h OGRe/variavel.h OGRe/multi-intervalo.h \
 OGRe/intervalo.h OGRe/lista.h OGRe/real.h OGRe/../constantes.h \
 OGRe/operacoes.h OGRe/heuristica.h OGRe/busca_local.h
busca_local.o: OGRe/busca_local.c OGRe/busca_local.h OGRe/epifita.h \
 OGRe/arvore_restricoes.h OGRe/variavel.h OGRe/multi-intervalo.h \
 OGRe/intervalo.h OGRe/lista.h OGRe/real.h OGRe/../constantes.h \
 OGRe/operacoes.h
dot.o: OGRe/dot.c OGRe/dot.h OGRe/epifita.h OGRe/arvore_restricoes.h \
 OGRe/variavel.h OGRe/multi-intervalo.h OGRe/intervalo.h OGRe/lista.h \
 OGRe/real.h OGRe/../constantes.h OGRe/operacoes.h
epifita.o: OGRe/epifita.c OGRe/epifita.h OGRe/arvore_restricoes.h \
 OGRe/variavel.h OGRe/multi-intervalo.h OGRe/intervalo.h OGRe/lista.h \
 OGRe/real.h OGRe/../constantes.h OGRe/operacoes.h
heuristica.o: OGRe/heuristica.c OGRe/heuristica.h OGRe/branch-bound.h \
 OGRe/epifita.h OGRe/arvore_restricoes.h OGRe/variavel.h \
 OGRe/multi-intervalo.h OGRe/intervalo.h OGRe/lista.h OGRe/real.h \
 OGRe/../constantes.h OGRe/operacoes.h OGRe/busca_local.h
intervalo.o: OGRe/intervalo.c OGRe/intervalo.h OGRe/lista.h OGRe/real.h \
 OGRe/../constantes.h
lista.o: OGRe/lista.c OGRe/lista.h
main.o: OGRe/main.c OGRe/ogre-de.h OGRe/../constantes.h OGRe/real.h \
 OGRe/branch-bound.h OGRe/epifita.h OGRe/arvore_restricoes.h \
 OGRe/variavel.h OGRe/multi-intervalo.h OGRe/intervalo.h OGRe/lista.h \
 OGRe/operacoes.h OGRe/heuristica.h OGRe/busca_local.h
multi-intervalo.o: OGRe/multi-intervalo.c OGRe/multi-intervalo.h \
 OGRe/intervalo.h OGRe/lista.h OGRe/real.h OGRe/../constantes.h
ogre-de.o: OGRe/ogre-de.c OGRe/real.h OGRe/branch-bound.h OGRe/epifita.h \
 OGRe/arvore_restricoes.h OGRe/variavel.h OGRe/multi-intervalo.h \
 OGRe/intervalo.h OGRe/lista.h OGRe/../constantes.h OGRe/operacoes.h \
 OGRe/heuristica.h OGRe/busca_local.h OGRe/../inter.h \
 OGRe/../constantes.h OGRe/ogre-de.h OGRe/dot.h
operacoes.o: OGRe/operacoes.c OGRe/operacoes.h
real.o: OGRe/real.c OGRe/real.h
rotacionador.o: OGRe/rotacionador.c OGRe/rotacionador_lib.h OGRe/real.h \
 OGRe/lista.h OGRe/intervalo.h OGRe/../constantes.h
rotacionador_lib.o: OGRe/rotacionador_lib.c OGRe/rotacionador_lib.h \
 OGRe/real.h OGRe/lista.h OGRe/intervalo.h OGRe/../constantes.h \
 OGRe/operacoes.h
variavel.o: OGRe/variavel.c OGRe/variavel.h OGRe/multi-intervalo.h \
 OGRe/intervalo.h OGRe/lista.h OGRe/real.h OGRe/../constantes.h
bbde_adaptParam.o: bbde/bbde_adaptParam.c bbde/bbde_adaptParam.h \
 bbde/../inter.h bbde/../constantes.h bbde/../constantes.h bbde/../rand.h \
 bbde/../OGRe/real.h bbde/../inter.h bbde/../alocDef.h bbde/bbde_uteis.h \
 bbde/bbde_operacoes.h
bbde_alocDef.o: bbde/bbde_alocDef.c bbde/bbde_alocDef.h bbde/../inter.h \
 bbde/../constantes.h bbde/../constantes.h
bbde_buscalocal.o: bbde/bbde_buscalocal.c bbde/bbde_buscalocal.h \
 bbde/../inter.h bbde/../constantes.h bbde/../rand.h bbde/../OGRe/real.h \
 bbde/../inter.h bbde/../alocDef.h bbde/../constantes.h \
 ../contratores.h bbde/bbde_uteis.h bbde/bbde_adaptParam.h \
 bbde/bbde_operacoes.h bbde/bbde_problema.h bbde/bbde_alocDef.h
bbde_operacoes.o: bbde/bbde_operacoes.c bbde/bbde_operacoes.h \
 bbde/../inter.h bbde/../constantes.h bbde/../constantes.h bbde/../rand.h \
 bbde/../OGRe/real.h bbde/../inter.h bbde/../alocDef.h bbde/bbde_uteis.h \
 bbde/bbde_adaptParam.h
bbde_problema.o: bbde/bbde_problema.c bbde/bbde_problema.h \
 bbde/../inter.h bbde/../constantes.h bbde/../constantes.h bbde/../rand.h \
 bbde/../OGRe/real.h bbde/../inter.h bbde/../alocDef.h \
 bbde/bbde_buscalocal.h ../contratores.h bbde/bbde_uteis.h \
 bbde/bbde_adaptParam.h bbde/bbde_operacoes.h bbde/bbde_alocDef.h \
 bbde/../instancia.h
bbde_uteis.o: bbde/bbde_uteis.c bbde/bbde_uteis.h bbde/../inter.h \
 bbde/../constantes.h bbde/../rand.h bbde/../OGRe/real.h bbde/../inter.h \
 bbde/../alocDef.h bbde/../constantes.h bbde/bbde_adaptParam.h \
 bbde/bbde_operacoes.h
adaptParam.o: adaptParam.c adaptParam.h inter.h constantes.h rand.h \
 OGRe/real.h alocDef.h uteis.h
alocDef.o: alocDef.c alocDef.h inter.h constantes.h
contratores.o: contratores.c contratores.h inter.h constantes.h
heap.o: heap.c heap.h inter.h
main.o: main.c main.h OGRe/real.h bbde/bbde_alocDef.h bbde/../inter.h \
 bbde/../constantes.h bbde/../constantes.h adaptParam.h inter.h rand.h \
 alocDef.h constantes.h uteis.h problema.h OGRe/ogre-de.h \
 OGRe/../constantes.h OGRe/real.h OGRe/branch-bound.h OGRe/epifita.h \
 OGRe/arvore_restricoes.h OGRe/variavel.h OGRe/multi-intervalo.h \
 OGRe/intervalo.h OGRe/lista.h OGRe/operacoes.h OGRe/heuristica.h \
 OGRe/busca_local.h bbde/bbde_buscalocal.h bbde/../rand.h \
 bbde/bbde_buscalocal.h bbde/bbde_uteis.h \
 bbde/bbde_adaptParam.h bbde/bbde_operacoes.h bbde/bbde_problema.h \
 bbde/bbde_alocDef.h
instancia.o: instancia.c
problema.o: problema.c problema.h inter.h constantes.h rand.h OGRe/real.h \
 alocDef.h uteis.h adaptParam.h main.h bbde/bbde_alocDef.h \
 bbde/../inter.h bbde/../constantes.h OGRe/ogre-de.h OGRe/../constantes.h \
 OGRe/real.h OGRe/branch-bound.h OGRe/epifita.h OGRe/arvore_restricoes.h \
 OGRe/variavel.h OGRe/multi-intervalo.h OGRe/intervalo.h OGRe/lista.h \
 OGRe/operacoes.h OGRe/heuristica.h OGRe/busca_local.h \
 bbde/bbde_buscalocal.h bbde/../rand.h bbde/../contratores.h \
 bbde/bbde_buscalocal.h bbde/bbde_uteis.h bbde/bbde_adaptParam.h \
 bbde/bbde_operacoes.h bbde/bbde_problema.h bbde/bbde_alocDef.h \
 instancia.h
rand.o: rand.c rand.h OGRe/real.h inter.h constantes.h alocDef.h
uteis.o: uteis.c uteis.h inter.h constantes.h adaptParam.h rand.h \
 OGRe/real.h alocDef.h
