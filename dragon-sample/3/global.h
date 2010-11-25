#include <stdio.h>
#include <ctype.h>

#define BSIZE 128
#define NONE -1
#define EOS '\0'

#define ID 258
#define NUM 259
#define DONE 261


int tokenval;
int lineno;

struct entry
{
	char *lexptr;
	int token;
};

extern struct entry symtable[];

