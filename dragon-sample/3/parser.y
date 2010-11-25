
%{
/* need this for the call to atof() below */
#include <math.h>
#include "global.h"
%}

%token ID
%token NUM

%%
list		:	expr ';' list
		|
		;
expr		:	term moreterms
		;
moreterms	:	'+' term {printf("+");} moreterms
		|	'-' term {printf("-");} moreterms
		|
		;
term		:	factor morefactors
		;
morefactors	:	'*' factor {printf("*");}  morefactors
		|	'/' factor {printf("/");}  morefactors
		|
		;
factor		:	'(' expr ')'
		|	ID {printf("id");}
		|	NUM {printf("num");}
		;
%%
