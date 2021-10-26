#include <cctype>
#include <fstream>
#include <cassert>

#include "verilog_driver.hpp"

DIAGNOSIS::VerilogDriver::~VerilogDriver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void
DIAGNOSIS::VerilogDriver::parse( const std::string &filename )
{
   std::ifstream in_file( filename );
   parse(in_file);
}

void
DIAGNOSIS::VerilogDriver::parse( std::istream &stream )
{
   if( ! stream.good()  && stream.eof() )
   {
       return;
   }
   //else
   parse_helper( stream ); 
   return;
}


void
DIAGNOSIS::VerilogDriver::parse_helper( std::istream &stream )
{
   
   delete(scanner);
   try
   {
      scanner = new DIAGNOSIS::VerilogScanner( &stream );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate scanner: (" <<
         ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }
   
   delete(parser); 
   try
   {
      parser = new DIAGNOSIS::VerilogParser( (*scanner) /* scanner */, 
                                  (*this) /* driver */ );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate parser: (" << 
         ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }
   const int accept( 0 );
   if( parser->parse() != accept )
   {
      std::cerr << "Parse failed!!\n";
   }
   return;
}

void DIAGNOSIS::VerilogDriver::addInputs(std::vector<std::string> &lst) {
    addAtomList(lst, INPUT);
}

void DIAGNOSIS::VerilogDriver::addAtomList(std::vector<std::string> &lst, DIAGNOSIS::CNFDriver::Type type) {
    for (auto & iter:lst){
        addAtom(iter,type);
    }

}

void DIAGNOSIS::VerilogDriver::addOutputs(std::vector<std::string> &lst) {
    addAtomList(lst, OUTPUT);
}

void DIAGNOSIS::VerilogDriver::addWires(std::vector<std::string> &lst) {
    addAtomList(lst, WIRE);
}



DIAGNOSIS::VerilogDriver::VerilogDriver(const std::string &luafile ):CNFDriver(luafile) {

}
