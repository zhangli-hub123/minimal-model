#ifndef __MCSCANNER_HPP__
#define __MCSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "verilog_parser.tab.hh"
#include "location.hh"

namespace DIAGNOSIS{

class VerilogScanner : public yyFlexLexer{
public:

    VerilogScanner(std::istream *in) : yyFlexLexer(in)
   {
   };
   virtual ~VerilogScanner() {
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( DIAGNOSIS::VerilogParser::semantic_type *  lval,
              DIAGNOSIS::VerilogParser::location_type *location );
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc


private:
   /* yyval ptr */
   DIAGNOSIS::VerilogParser::semantic_type *yylval = nullptr;
};

} /* end namespace MC */

#endif /* END __MCSCANNER_HPP__ */
