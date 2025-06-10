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
 *  Pré-processador que rotaciona a Árvore Epífita.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "operacoes.h"
#include <string.h>

int opcode(char *operacao) {

	if (!strcmp(operacao, "OP"))
		return OP;
	else if (!strcmp(operacao, "IOP"))
		return IOP;
	else if (!strcmp(operacao, "COP"))
		return COP;
	else if (!strcmp(operacao, "ADD"))
		return ADD;
	else if (!strcmp(operacao, "SUB"))
		return SUB;
	else if (!strcmp(operacao, "MUL"))
		return MUL;
	else if (!strcmp(operacao, "DIV"))
		return DIV;
	else if (!strcmp(operacao, "POW"))
		return POW;
	else if (!strcmp(operacao, "SQR"))
		return SQR;
	else if (!strcmp(operacao, "LG"))
		return LG;
	else if (!strcmp(operacao, "SIN"))
		return SIN;
	else if (!strcmp(operacao, "ARCS"))
		return ARCS;
	else if (!strcmp(operacao, "CSIN"))
		return CSIN;
	else if (!strcmp(operacao, "COS"))
		return COS;
	else if (!strcmp(operacao, "ARCC"))
		return ARCC;
	else if (!strcmp(operacao, "CCOS"))
		return CCOS;
	else if (!strcmp(operacao, "TAN"))
		return TAN;
	else if (!strcmp(operacao, "ARCT"))
		return ARCT;
	else if (!strcmp(operacao, "CTAN"))
		return CTAN;
	else if (!strcmp(operacao, "EXP"))
		return EXP;
	else if (!strcmp(operacao, "LOG"))
		return LOG;
	else if (!strcmp(operacao, "CEXP"))
		return CEXP;
	else if (!strcmp(operacao, "ABS"))
		return ABS;
	else if (!strcmp(operacao, "IABS"))
		return IABS;
	else if (!strcmp(operacao, "CABS"))
		return CABS;
	else if (!strcmp(operacao, "SGN"))
		return SGN;
	else if (!strcmp(operacao, "VAL"))
		return VAL;
	else if (!strcmp(operacao, "CSGN"))
		return CSGN;
	else if (!strcmp(operacao, "MAX"))
		return MAX;
	else if (!strcmp(operacao, "IMAX"))
		return IMAX;
	else if (!strcmp(operacao, "CMAX"))
		return CMAX;
	else if (!strcmp(operacao, "MIN"))
		return MIN;
	else if (!strcmp(operacao, "IMIN"))
		return IMIN;
	else if (!strcmp(operacao, "CMIN"))
		return CMIN;
	else
		return -1;
}

char *opcode2str(int op) {

	if (op == OP)
		return "OP";
	else if (op == IOP)
		return "IOP";
	else if (op == COP)
		return "COP";
	else if (op == ADD)
		return "ADD";
	else if (op == SUB)
		return "SUB";
	else if (op == MUL)
		return "MUL";
	else if (op == DIV)
		return "DIV";
	else if (op == POW)
		return "POW";
	else if (op == SQR)
		return "SQR";
	else if (op == LG)
		return "LG";
	else if (op == SIN)
		return "SIN";
	else if (op == ARCS)
		return "ARCS";
	else if (op == CSIN)
		return "CSIN";
	else if (op == COS)
		return "COS";
	else if (op == ARCC)
		return "ARCC";
	else if (op == CCOS)
		return "CCOS";
	else if (op == TAN)
		return "TAN";
	else if (op == ARCT)
		return "ARCT";
	else if (op == CTAN)
		return "CTAN";
	else if (op == EXP)
		return "EXP";
	else if (op == LOG)
		return "LOG";
	else if (op == CEXP)
		return "CEXP";
	else if (op == ABS)
		return "ABS";
	else if (op == IABS)
		return "IABS";
	else if (op == CABS)
		return "CABS";
	else if (op == SGN)
		return "SGN";
	else if (op == VAL)
		return "VAL";
	else if (op == CSGN)
		return "CSGN";
	else if (op == MAX)
		return "MAX";
	else if (op == IMAX)
		return "IMAX";
	else if (op == CMAX)
		return "CMAX";
	else if (op == MIN)
		return "MIN";
	else if (op == IMIN)
		return "IMIN";
	else if (op == CMIN)
		return "CMIN";
	else
		return "???";
}
