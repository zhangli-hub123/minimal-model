#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>
#include <vector>

#include "verilog_scanner.hpp"
#include "verilog_parser.tab.hh"
#include "cnf_driver.h"
#include "lua.hpp"
namespace DIAGNOSIS{

class VerilogDriver: public CNFDriver{
public:
    VerilogDriver(const std::string &luafile );

    ~VerilogDriver();
   void addInputs(std::vector<std::string>&);
   void addOutputs(std::vector<std::string>&);
   void addWires(std::vector<std::string>&);

   /** 
    * parse - parse from a file
    * @param filename - valid string with input file
    */
   void parse( const std::string &filename );
   /** 
    * parse - parse from a c++ input stream
    * @param is - std::istream&, valid input stream
    */
   void parse( std::istream &iss );

private:

   void parse_helper( std::istream &stream );
   void addAtomList(std::vector<std::string>&,Type type);

   DIAGNOSIS::VerilogParser  *parser  = nullptr;
   DIAGNOSIS::VerilogScanner *scanner = nullptr;

};

} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
