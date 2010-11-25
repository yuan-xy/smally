#include "global.h"

/**
	gcc main.c error.c init.c symbol.c emitter.c parser.c lexer.c
*/
int main(){
	init();
	yyparse();
	exit(0);
	return 0;
}
