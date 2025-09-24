#ifndef _INSTANCIA_H_
#define _INSTANCIA_H_

/* returns the number of variables and constraints */
void problem_size(int *nvar, int *nconstr);

/* returns the domain of the variables */
void problem_dom(double *dominf, double *domsup);

/* evaluates the problem using variable values in var and */
/* returns the value of the objective function in val and */
/* and the constraint violations in array constr          */
void problem_eval(double *var, double *val, double *constr);

#endif
