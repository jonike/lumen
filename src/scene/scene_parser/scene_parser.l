%{
#define YY_NO_UNISTD_H

#include <cstdlib>
#include "token.h"
#include "scene_parser.tab.h"

void yyerror(const char*);
%}

%option never-interactive
%option noyywrap

%x ERROR
%%

\"(\\.|[^"\n])*\" {
        yylval.strval = yytext;
        yylval.strval = yylval.strval.substr(1, yylval.strval.substr().size() - 2);

        return STRING;
}

-?[0-9]+(\.[0-9]+)? {
        yylval.floatval = static_cast<float>(atof(yytext));
        return NUMBER;
}

Display         { return DISPLAY; }
Resolution      { return RESOLUTION; }
PixelSamples    { return PIXELSAMPLES; }
DepthOfField    { return DEPTHOFFIELD; }
WorldBegin      { return WORLDBEGIN; }
WorldEnd        { return WORLDEND; }
Geometry        { return GEOMETRY; }
ObjectBegin     { return OBJECTBEGIN; }
ObjectEnd       { return OBJECTEND; }
Light           { return LIGHT; }
Material        { return MATERIAL; }
Projection      { return PROJECTION; }
Translate       { return TRANSLATE; }
Rotate          { return ROTATE; }
Scale           { return SCALE; }

\[              { return '['; }
\]              { return ']'; }

\n              { ++yylineno; }

#.* ;
[ \t]+ ;

. {
        BEGIN(ERROR);
        yymore();
}

<ERROR>[^ \t\n]+ {
        yymore();
}

<ERROR>[ \t\n] {
        yyless(yyleng - 1);
        yyerror("syntax error");
        BEGIN(INITIAL);
}
%%
