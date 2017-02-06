%{
#include <stdio.h>
#include <string.h>
#include "node.hpp"
#include "parser.hpp"

static int n_names = 0;
static int n_learners = 0;


%}
%option noyywrap
%start NAMELIST LEARNERLIST STRSTART 
%%
<INITIAL>[ \t]	    		;
<INITIAL>#.*\n		 		;
<INITIAL>.					{ printf("encounter unexpected char [%c]\n", yytext[0]); }
<INITIAL>"names="     		{ /*printf("-->MODE[namelist]: \n");*/ BEGIN NAMELIST; return NAMES; }
<NAMELIST>[a-zA-Z_][a-zA-Z_0-9]*  { 
								n_names++; /*printf("\t[%d]->%s\n", n_names, yytext);*/ 
								yylval.pstr = new std::string(yytext); 
								return STRING; 
							}
<NAMELIST>[ \t]         	;
<NAMELIST>\n				{ /*printf("<--MODE[namelist]\n");*/ BEGIN INITIAL; }
<NAMELIST>.					{ printf("Unexpected char[%c] in namelist\n", yytext[0]); BEGIN INITIAL; }
<INITIAL>"beforeloop="		{ BEGIN STRSTART; yylval.tag = BLOOP; return TAG;}
<INITIAL>"beforeloopinit="	{ BEGIN STRSTART; yylval.tag = BLOOPI; return TAG; }
<INITIAL>"precondition="    { BEGIN STRSTART; yylval.tag = PREC; return TAG; }
<INITIAL>"loopcondition="   { BEGIN STRSTART; yylval.tag = LOOPC; return TAG; }
<INITIAL>"loop="        	{ BEGIN STRSTART; yylval.tag = LOOP; return TAG; }
<INITIAL>"postcondition="	{ BEGIN STRSTART; yylval.tag = PSTC; return TAG; }
<INITIAL>"afterloop="       { BEGIN STRSTART; yylval.tag = ALOOP; return TAG; }
<INITIAL>"invariant="       { BEGIN STRSTART; yylval.tag = INVR; /*printf("encounter INVARIANT.\n");*/ return TAG; }
<STRSTART>[^\n]*\n			{ yytext[yyleng-1] = '\0';
							  /*printf("\t[%d]->%s\n", yylval.tag, yytext);*/ 
							  yylval.pstr = new std::string(yytext); 
							  BEGIN INITIAL;
							  return STRING; 
							}

<INITIAL>"learners="        { BEGIN LEARNERLIST; return LEARNERS; }
<LEARNERLIST>[a-zA-Z_][a-zA-Z_0-9]*  { 
								n_learners++; 
								/*printf("\t[learner]->%s\n", yytext);*/ 
								yylval.pstr = new std::string(yytext); 
								return STRING; 
							}
<LEARNERLIST>[ \t]         	;
<LEARNERLIST>\n				{ BEGIN INITIAL; }
<LEARNERLIST>.				{ printf("Unexpected char[%c] in learnerlist\n", yytext[0]); BEGIN INITIAL; }
%%
