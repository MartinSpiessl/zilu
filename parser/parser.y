%{
	#include "node.hpp"
	#include <string>

extern int yylex();
int yyerror(char const *str);


extern Loop* loop;
extern const std::string tag_name[];

%}

/* Represents the many different ways we can access our data */
%union {
	tag_type tag;
	std::string* pstr;
	NameList* names;
	NameList* symbs;
	NameList* learners;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <tag> NAMES LEARNERS SYMBS TAG
%token <pstr> STRING
%type <names> namelist symblist learnerlist
/*%type <nodes> stmtlist*/
/*%type <loop_type> stmtlist*/
%start root

%%
root	: namelist stmtlist
		{ 
			//printf("reduce to root.\n"); 
			loop->names = $1;
			loop->learners = new NameList();
		}
		| namelist symblist stmtlist
		{ 
			//printf("reduce to root.\n"); 
			loop->names = $1;
			loop->symbs = $2;
			loop->learners = new NameList();
		}
		| namelist stmtlist learnerlist stmtlist
		{ 
			//printf("reduce to root.\n"); 
			loop->names = $1;
			loop->symbs = new NameList();
			loop->learners = $3;
		}
		| namelist stmtlist symblist stmtlist learnerlist stmtlist
		{ 
			//printf("reduce to root.\n"); 
			loop->names = $1;
			loop->symbs = $3;
			loop->learners = $5;
		}
		;

namelist: namelist STRING
		{
			$$ = $1;
			$$->addName(*$2);
			//cout << "addName<" << *$2 << ">" << endl;
		}
		| NAMES STRING
		{ 
			$$ = new NameList();
			$$->addName(*$2);
			//cout << "addName<" << *$2 << ">" << endl;
		}
		;

symblist: symblist STRING
		{
			//cout << "addSYM<" << *$2 << ">" << endl;
			$$ = $1;
			$$->addName(*$2);
		}
		| SYMBS STRING
		{ 
			//cout << "addSYM<" << *$2 << ">" << endl;
			$$ = new NameList();
			$$->addName(*$2);
		}
		;

learnerlist: learnerlist STRING
		{
			$$ = $1;
			$$->addName(*$2);
			//cout << "addName<" << *$2 << ">" << endl;
		}
		| LEARNERS STRING
		{ 
			$$ = new NameList();
			$$->addName(*$2);
			//cout << "addName<" << *$2 << ">" << endl;
		}
		;

stmtlist: stmtlist TAG STRING 
		{ 
			//cout << "addItem [" << tag_name[$2] << "] ## <" << *$3 << ">" << endl;
			loop->addItem($2, $3);
		}
		|
		;
%%

int yyerror(char const *str)
{
	//cout << "in yyerror.\n";
	extern char* yytext;
	fprintf(stderr, "parser error near \"%s\"\n", yytext);
	return 0;
}

