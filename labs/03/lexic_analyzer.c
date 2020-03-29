%{
#include <stdio.h>
#include <stdlib.h>
%}

%%
\/\/(.+)        ;
f               { fprintf(yyout, "floatdcl"); }
i               { fprintf(yyout, "intdcl"); }
p               { fprintf(yyout, "print"); }
[a-eghj-oq-z]   { fprintf(yyout, "id"); }
[0-9]+\.[0-9]+  { fprintf(yyout, "fnum"); }
[0-9]+          { fprintf(yyout, "inum"); }
=               { fprintf(yyout, "assign"); }
\+              { fprintf(yyout, "plus"); }
-               { fprintf(yyout, "minus"); }
%%

int main(int argc, char *argv[])
{   
	extern FILE *yyin, *yyout;
    if( argc == 2)
	{
        yyin = fopen(argv[1], "r");
        yyout = fopen("lex.out", "w");
        yylex();
    }
    else { printf("Invalid number of arguments.\n"); }
    fclose(yyin);
    fclose(yyout);
}
