%{
// Lexer
#include <stdio.h>
#include <string.h>
#include "parser.h"
%}

%s _SERVER
%s _PORT
%s _MAX_CONNECTIONS
%s _USE_SOCKS
%s _SOCKS_PROTO
%s _SOCKS_HOST
%s _SOCKS_PORT
%s _SOCKS_USERNAME
%s _SOCKS_PASSWORD
%s _SOCKS_DNS

digit		[0-9]
number		{digit}+
letter      [a-zA-Z]
word        {letter}+
hostpart    [a-zA-Z0-9\-\.]
hostname    {letter}{hostpart}+
ip          {number}\.{number}\.{number}\.{number}
any			.*

%%

#.*			; /* Eat Comments */
[\t ]		; /* eat whitespace */
\n			BEGIN INITIAL;

<INITIAL>[Ss][Ee][Rr][Vv][Ee][Rr]														{BEGIN _SERVER; return SERVER;}
<INITIAL>[Mm][Aa][Xx][_][Cc][Oo][Nn][Nn][Ee][Cc][Tt][Ii][Oo][Nn][Ss]					{BEGIN _MAX_CONNECTIONS; return MAX_CONNECTIONS;}
<INITIAL>[Uu][Ss][Ee][_][Ss][Oo][Cc][Kk][Ss]											{BEGIN _USE_SOCKS; return USE_SOCKS;}
<INITIAL>[Ss][Oo][Cc][Kk][Ss][_][Pp][Rr][Oo][Tt][Oo]									{BEGIN _SOCKS_PROTO; return SOCKS_PROTO;}
<INITIAL>[Ss][Oo][Cc][Kk][Ss][_][Hh][Oo][Ss][Tt]										{BEGIN _SOCKS_HOST; return SOCKS_HOST;}
<INITIAL>[Ss][Oo][Cc][Kk][Ss][_][Pp][Oo][Rr][Tt]										{BEGIN _SOCKS_PORT; return SOCKS_PORT;}
<INITIAL>[Ss][Oo][Cc][Kk][Ss][_][Uu][Ss][Ee][Rr][Nn][Aa][Mm][Ee]						{BEGIN _SOCKS_USERNAME; return SOCKS_USERNAME;}
<INITIAL>[Ss][Oo][Cc][Kk][Ss][_][Pp][Aa][Ss][Ss][Ww][Oo][Rr][Dd]						{BEGIN _SOCKS_PASSWORD; return SOCKS_PASSWORD;}
<INITIAL>[Ss][Oo][Cc][Kk][Ss][_][Dd][Nn][Ss]											{BEGIN _SOCKS_DNS; return SOCKS_DNS;}

<_SERVER>:																				{BEGIN _PORT; return PORT;}

<_SERVER>{ip}																			{yylval.string = strdup(yytext);return IPADDR;}
<_SERVER>{hostname}																		{yylval.string = strdup(yytext);return HOSTNAME;}
<_PORT>{number}																			{yylval.number = strtol(yytext, NULL, 10); return PORTNUM;}

<_MAX_CONNECTIONS>{number}																{yylval.number = strtol(yytext, NULL, 10); return NUMBER;}

<_USE_SOCKS>{number}																	{yylval.number = strtol(yytext, NULL, 10); return NUMBER;}
<_SOCKS_DNS>{number}																	{yylval.number = strtol(yytext, NULL, 10); return NUMBER;}
<_SOCKS_PROTO>{number}																	{yylval.number = strtol(yytext, NULL, 10); return NUMBER;}
<_SOCKS_PORT>{number}																	{yylval.number = strtol(yytext, NULL, 10); return PORTNUM;}

<_SOCKS_HOST>{hostname}																	{yylval.string = strdup(yytext); return HOSTNAME;}
<_SOCKS_HOST>{ip}																		{yylval.string = strdup(yytext); return IPADDR;}
<_SOCKS_USERNAME>{any}																	{yylval.string = strdup(yytext); return STRING;}
<_SOCKS_PASSWORD>{any}																	{yylval.string = strdup(yytext); return STRING;}

%%
