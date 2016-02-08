%{
#include <iostream>
#include "obj_lexer.h"
#include "debug.h"

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

{COMMENT} { DBG(std::cout << "Got comment : " << yytext << std::endl;) }
mtllib { DBG(std::cout << "Got mtllib token " << std::endl;)
         yylval->build<Keyword>(Keyword::MTLLIB);
         return token::TOK_MTLLIB; }
usemtl { DBG(std::cout << "Got usemtl token " << std::endl;)
         yylval->build<Keyword>(Keyword::USEMTL);
         return token::TOK_USEMTL; }
o       { DBG(std::cout << "Got object token " << std::endl;)
          yylval->build<Keyword>(Keyword::OBJECT);
          return token::TOK_OBJECT; }
vn      { DBG(std::cout << "Got vertex normal token " << std::endl;)
          yylval->build<Keyword>(Keyword::NORMAL);
          return token::TOK_NORMAL; }
v       { DBG(std::cout << "Got vertex token " << std::endl;)
          yylval->build<Keyword>(Keyword::VERTEX);
          return token::TOK_VERTEX; }
g       { DBG(std::cout << "Got group token " << std::endl;)
          yylval->build<Keyword>(Keyword::GROUP);
          return token::TOK_GROUP; }
s       { DBG(std::cout << "Got smoothing group token " << std::endl;)
          yylval->build<Keyword>(Keyword::SMOOTH);
          return token::TOK_SMOOTH; }
f       { DBG(std::cout << "Got face token " << std::endl;)
          yylval->build<Keyword>(Keyword::FACE);
          return token::TOK_FACE; }
{FLOAT_LITERAL} { DBG(std::cout << "Got single coord : " << yytext << std::endl;)
                  yylval->build<float>(std::stof(yytext));
                  return token::TOK_FLOAT; }
{INT_LITERAL} { DBG(std::cout << "Got integer : " << yytext << std::endl;)
                  yylval->build<int>(std::stoi(yytext));
                  return token::TOK_INT; }
{IDENT}.mtl { DBG(std::cout << "Got .mtl file : " << yytext << std::endl;)
                 yylval->build<std::string>(yytext);
                 return token::TOK_IDENT; }
{IDENT} { DBG(std::cout << "Got object name : " << yytext << std::endl;)
             yylval->build<std::string>(yytext);
             return token::TOK_IDENT; }
\/ { yylval->build<char>(yytext[0]); return token::TOK_SLASH; }
[[:space:]] { }
. { LexerError("Unknown token"); }

%%
