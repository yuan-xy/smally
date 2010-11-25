#include "global.h"

/**
	flex test.flex
	gcc main.c error.c init.c symbol.c emitter.c parser.c lex.yy.c -lfl
*/
int main(){
	init();
	parse();
	exit(0);
	return 0;
}
