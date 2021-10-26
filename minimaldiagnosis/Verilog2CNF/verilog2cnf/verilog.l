%{
/* 这里的部分会被直接拷贝到生成的 .c 文件的开始部分，在这里可以包含需要使用的头文件，如 stdio.h
*/
#include <stdio.h>
#include "verilog_parser.tab.hh"
#include "verilog_scanner.hpp"
#define yyterminate() return( token::MODULE_DEFINE)
#undef  YY_DECL
#define YY_DECL int DIAGNOSIS::VerilogScanner::yylex( DIAGNOSIS::VerilogParser::semantic_type * const lval, DIAGNOSIS::VerilogParser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = DIAGNOSIS::VerilogParser::token;

%}

%option debug
%option nodefault
%option yyclass="DIAGNOSIS::VerilogScanner"
%option noyywrap
%option c++
_ID [a-zA-Z][a-zA-Z_0-9]*
%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

"module" {return token::MODULE_DEFINE;}
"endmodule" { return token::MODULE_END;}
"input" {return token::INPUT;}
"output"  {return token::OUTPUT;}
"wire" {return token::WIRE;}
 "(" {return token::LP;}
 ")" {return token::RP;}
 ";" {return  token::SEM;}
 "," {return token::COMMA;}
[a-zA-Z][a-zA-Z_0-9]* {yylval->build< std::string >( yytext ); return token::ID; }
 [ \t] ;
 "\n" {loc->lines();}
   "//".* ;
 . ;
%%
