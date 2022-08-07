%{
    #define MAX_ARGUMENT_NUMBER 128
    char *_argvector[MAX_ARGUMENT_NUMBER];	// 解析参数
    int _argcounter = 0;					// 参数个数
%}

WORD	[a-zA-Z0-9\/\.\-~]+
STRINGLITERAL \"(\\.|[^\\"])*\"
SPECIAL	[()><|&*]

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

\n	return (int)_argvector;	// 解析到换行符时结束

[ \t]+

\#[^\n]*	;	// 忽略以#开头的注释

. {
  char str[128] = {0};
  sprintf(str, "Unrecognized token [%s] in input sql.", yytext);
//   ParserSetError(str);
}

%%

int yywrap()
{
	return 1;
}

int yy_lexer(int *argc, char ***argv)
{
	yylex();
	
	*argc = _argcounter;
	*argv = _argvector;
	
	return 0;
}
