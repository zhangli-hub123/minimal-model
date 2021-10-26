#include <iostream>
#include <vector>
#include "verilog_driver.hpp"
#include <getopt.h>
#include <fstream>
#define check_empty(value,option)     if (value.empty()){std::cerr<<option<<" can not be empty"<<std::endl;usage();exit(0);}

static struct option long_options[] = {
        { "lua", 1, 0, 'l'},
        { "type", 0, 0, 't'},
        { "input", 1, 0, 'i'},
        { "output", 1, 0, 'o'},
        { "help", 0, 0, 'h'},
        {0, 0, 0, 0},
};
static char optstring[] = "l:t:i:o:h";

template<typename T>
void _do(std::string &lua,std::string &input,std::string &output);
void usage();
int main(int argc, char* argv[]) {
    std::string  lua_path;
    std::string type="verilog";
    std::string input;
    std::string output;
    while(1){
        int c = getopt_long(argc, argv, optstring, long_options, NULL);
        if (c==-1)break;
        switch (c){
            case 'l':
                lua_path=optarg;
                break;
            case 't':
                type= optarg;
                break;
            case 'i':
                input= optarg;
                break;
            case 'o':
                output= optarg;
                break;
            case 'h':
                usage();
                return 0;
            default :
                usage();
                return -1;
        }
    }
    check_empty(lua_path,"--lua")
    check_empty(input,"-i/--input")
    if (lua_path.empty()){
        std::cerr<<"--lua is not empty"<<std::endl;
        return-1;
    }
    if (input.empty()){
        std::cerr<<"--input is not empty"<<std::endl;
        return-1;
    }
    DIAGNOSIS::VerilogDriver driver(lua_path);
    if (type=="verilog"){
        _do<DIAGNOSIS::VerilogDriver>(lua_path,input,output);
    }
    return 0;
}
template<typename T>
void _do(std::string &lua,std::string &input,std::string &output){
    T driver(lua);
    driver.parse(input);
    if (!output.empty()){
        std::ofstream ofs(output,std::ios_base::out|std::ios_base::trunc);
        driver.print(ofs);
        ofs.close();
    } else{
        driver.print(std::cout);
    }
    exit(0);
}

void usage(){
    using namespace std;
    cout<< "usage: "<<endl;
    cout<< "       circuit2cnf --input/-i <input file> --lua <lua file> [other options]..."<<endl;
    cout<< "options:"<<endl;
    cout<< "    -i, --input   circuit file"<<endl;
    cout<< "        --lua     lua file for helping parse circuit "<<endl;
    cout<< "    -o, --output  result file"<<endl;
    cout <<"    -h, --help    print help message"<<endl;
}
