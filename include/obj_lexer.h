#ifndef OBJ_LEXER_H
#define OBJ_LEXER_H

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int ObjLexer::yylex()

#include "obj_parser.h"

class ObjLexer : public yyFlexLexer {
    public:
        ObjLexer(std::istream *in) :
            yyFlexLexer(in),
            yylval(nullptr)
        { }

        int yylex(yy::ObjParser::semantic_type *lval) {
            yylval = lval;
            return yylex();
        }
        
    private:
        int yylex();
        yy::ObjParser::semantic_type* yylval;

        /* This class is not copyable */
        ObjLexer(ObjLexer const& other);
        ObjLexer& operator=(ObjLexer const& other);
};

enum class Keyword { MTLLIB, OBJECT, VERTEX, NORMAL, USEMTL, SMOOTH, FACE, GROUP };

#endif
