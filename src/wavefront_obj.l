%{
#include <iostream>
#include "obj_lexer.h"

// Make return statements shorter...
typedef yy::ObjParser::token_type token;

%}

%option nodefault
%option noyywrap
%option yylineno
%option yyclass="ObjLexer"
%option c++

COMMENT #.*$
IDENT ([[:alnum:]]|[_.-])+
FLOAT_LITERAL (-?[0-9]+\.[0-9]*|-?[0-9]+\.[0-9]*e[+-]?[0-9]+)
INT_LITERAL [+-]?(0|[1-9][0-9]*)

%%

{COMMENT} { }
mtllib { yylval->build<Keyword>(Keyword::MTLLIB);
         return token::TOK_MTLLIB; }
usemtl { yylval->build<Keyword>(Keyword::USEMTL);
         return token::TOK_USEMTL; }
o       { yylval->build<Keyword>(Keyword::OBJECT);
          return token::TOK_OBJECT; }
vn      { yylval->build<Keyword>(Keyword::NORMAL);
          return token::TOK_NORMAL; }
v       { yylval->build<Keyword>(Keyword::VERTEX);
          return token::TOK_VERTEX; }
g       { yylval->build<Keyword>(Keyword::GROUP);
          return token::TOK_GROUP; }
s       { yylval->build<Keyword>(Keyword::SMOOTH);
          return token::TOK_SMOOTH; }
f       { yylval->build<Keyword>(Keyword::FACE);
          return token::TOK_FACE; }
{FLOAT_LITERAL} { yylval->build<float>(std::stof(yytext));
                  return token::TOK_FLOAT; }
{INT_LITERAL} { yylval->build<int>(std::stoi(yytext));
                return token::TOK_INT; }
{IDENT}.mtl { yylval->build<std::string>(yytext);
              return token::TOK_IDENT; }
{IDENT} { yylval->build<std::string>(yytext);
          return token::TOK_IDENT; }
\/ { yylval->build<char>(yytext[0]); return token::TOK_SLASH; }
[[:space:]] { }
. { LexerError("Unknown token"); }

%%
