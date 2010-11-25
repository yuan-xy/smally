/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <math.h>
#include "global.h"
%}

DIGIT    [0-9]
I_D       [a-z][a-z0-9]*

%%

{DIGIT}+    {
		tokenval=atoi( yytext );return NUM;
            }
{I_D}        {
		int p=lookup(yytext);
		if(p==0) p=insert(yytext,ID);return symtable[p].token;
            }
\n          ++lineno;
[ \t\n]+    /* eat up whitespace */
\0		return DONE;
.           {
		tokenval=NONE;return yytext[0];
	    }
%%
