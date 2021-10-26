%skeleton "lalr1.cc"
%require  "3.0"
%defines
%error-verbose
%define api.namespace {DIAGNOSIS}
%define api.parser.class {VerilogParser}
%code requires{
   namespace DIAGNOSIS {
      class VerilogDriver;
      class VerilogScanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}
%parse-param { VerilogScanner  &scanner  }
%parse-param { VerilogDriver  &driver  }
%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
#include <vector>
#include <vector>
   /* include for all driver functions */
   #include "verilog_driver.hpp"

#undef yylex
#define yylex scanner.yylex
}
%locations
%define api.value.type variant
%define parse.assert
%token MODULE_DEFINE MODULE_END
%token INPUT OUTPUT WIRE
%token <std::string> ID
%token LP RP CR SEM COMMA
%type <std::vector<std::string>*> params

%%
rules: module
| rules module
;
module:
      MODULE_DEFINE ID LP params RP SEM {delete $4;}
      | INPUT params SEM {driver.addInputs(*$2);delete $2;}
      | OUTPUT params SEM {driver.addOutputs(*$2);delete $2;}
      | WIRE params SEM {driver.addWires(*$2);delete $2;}
      | ID ID LP params RP SEM { std::vector<std::string>*tmp=$4; driver.parseRule($1,$2,*tmp); delete tmp;}
      | MODULE_END {return 0;}
      ;
params:
      ID {std::vector<std::string> * lst=new std::vector<std::string>();lst->push_back($1);$$=lst;}
      | params COMMA ID {std::vector<std::string> * lst=$1;lst->push_back($3);$$=lst;}
      ;
%%
void
DIAGNOSIS::VerilogParser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: this file is not verilog file\n";
}