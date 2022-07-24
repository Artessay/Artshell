%{
    #include "lexer.h"
    char *_argvector[MAX_ARGUMENT_NUMBER];
    int _argcounter = 0;
%}

WORD	[a-zA-Z0-9\/\.-]+
STRINGLITERAL \"(\\.|[^\\"])*\"
SPECIAL	[()><|&;*]

%%
	_argcounter = 0; 
	_argvector[0] = NULL; 

{WORD}|{SPECIAL}|{STRINGLITERAL} {
	if(_argcounter < MAX_ARGUMENT_NUMBER-1) 
	{
	    _argvector[_argcounter++] = (char *)strdup(yytext);
	    _argvector[_argcounter] = NULL;
	}
}

\n	;

[ \t]+



.

%%

void yy_lexer(int argc, char **argv)
{
	yylex();
	
	argc = _argcounter;
	argv = _argvector;
	
	return;
}
