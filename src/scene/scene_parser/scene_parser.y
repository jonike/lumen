%{
#include <cstdlib>
#include <iostream>
#include "scene.h"
#include "token.h"

extern FILE* yyin;
extern char* yytext;
extern int   yylineno;

int  yylex();
int  yylex_destroy();
void yyerror(const char*);

std::vector<lumen::parameter> cur_params;
std::vector<lumen::parameter> no_params;
%}

%token <strval>   STRING
%token <floatval> NUMBER

%token DISPLAY RESOLUTION PIXELSAMPLES DEPTHOFFIELD
%token WORLDBEGIN WORLDEND
%token GEOMETRY OBJECTBEGIN OBJECTEND
%token LIGHT
%token MATERIAL
%token PROJECTION TRANSLATE ROTATE SCALE

%type <paramval> list
%type <paramval> array
%type <paramval> type
%type <paramval> parameter
%%

scene: scene command | command;

command: Display | Resolution | PixelSamples | DepthOfField |
         WorldBegin | WorldEnd |
         Geometry | ObjectBegin | ObjectEnd |
         Light |
         Material |
         Projection | Translate | Rotate | Scale;

Display:        DISPLAY STRING                      { lumen::Display($2.c_str()); }
Resolution:     RESOLUTION NUMBER NUMBER            { lumen::Resolution((int)$2, (int)$3); }
PixelSamples:   PIXELSAMPLES NUMBER NUMBER          { lumen::PixelSamples((int)$2, (int)$3); }
DepthOfField:   DEPTHOFFIELD NUMBER NUMBER NUMBER   { lumen::DepthOfField($2, $3, $4); }

WorldBegin:     WORLDBEGIN                          { lumen::WorldBegin(); }
WorldEnd:       WORLDEND                            { lumen::WorldEnd(); }

Geometry:       GEOMETRY STRING parameter_list      { lumen::Geometry($2.c_str(), cur_params); } |
                GEOMETRY STRING                     { lumen::Geometry($2.c_str(), no_params); }

ObjectBegin:    OBJECTBEGIN                         { lumen::ObjectBegin(); }
ObjectEnd:      OBJECTEND                           { lumen::ObjectEnd(); }

Light:          LIGHT STRING parameter_list         { lumen::Light($2.c_str(), cur_params); } |
                LIGHT STRING                        { lumen::Light($2.c_str(), no_params); }

Material:       MATERIAL STRING parameter_list      { lumen::Material($2.c_str(), cur_params); } |
                MATERIAL STRING                     { lumen::Material($2.c_str(), no_params); }

Projection:     PROJECTION NUMBER                   { lumen::Projection($2); }
Translate:      TRANSLATE NUMBER NUMBER NUMBER      { lumen::Translate($2, $3, $4); }
Rotate:         ROTATE NUMBER NUMBER NUMBER NUMBER  { lumen::Rotate($2, $3, $4, $5); }
Scale:          SCALE NUMBER NUMBER NUMBER          { lumen::Scale($2, $3, $4); }

list:           list NUMBER {
                        $1.arrayval.push_back($2);
                        $$ = $1;
                } |
                NUMBER {
                        $$.arrayval.push_back($1);
                };

array:          '[' list ']' { $$ = $2; };

type:           NUMBER {
                        $$.floatval = $1;
                } |
                array {
                        $$ = $1;
                } |
                STRING {
                        $$.strval = $1;
                };

parameter:      STRING type {
                        $2.name = $1;
                        $$ = $2;
                };

parameter_list: parameter_list parameter {
                        cur_params.push_back($2);
                } |
                parameter {
                        cur_params.clear();
                        cur_params.push_back($1);
                };
%%

static std::string get_filename(std::string filepath);

int main(int argc, char** argv)
{
        std::string program(get_filename(argv[0]));

        if (argc == 1) {
                std::cout << "Usage: " << program << " FILE" << std::endl;
                return EXIT_FAILURE;
        }

        FILE* file;
        errno_t err = fopen_s(&file, argv[1], "r");

        if (err) {
                char err_str[256];
                strerror_s(err_str, sizeof(err_str), err);

                std::cout << program << ": " << argv[1] << ": " << err_str << std::endl;

                return EXIT_FAILURE;
        }

        lumen::Begin();

        yyin = file;

        try {
                yyparse();
        } catch (const std::exception& err) {
                std::cout << err.what() << std::endl;
        }

        yylex_destroy();

        lumen::End();

        fclose(file);

        return EXIT_SUCCESS;
}

void yyerror(const char* s)
{
        printf("%s: line %d: %s\n", s, yylineno, yytext);
        exit(EXIT_FAILURE);
}

static std::string get_filename(std::string filepath)
{
        size_t slash = filepath.find_last_of("/\\");

        if (slash != std::string::npos) {
                filepath = filepath.substr(slash + 1);
        }

        size_t dot = filepath.find_last_of(".");

        if (dot != std::string::npos) {
                filepath = filepath.substr(0, dot);
        }

        return filepath;
}
