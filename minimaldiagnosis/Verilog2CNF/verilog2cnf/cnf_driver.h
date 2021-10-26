//
// Created by zhangli on 2020/11/26.
//

#ifndef DIAGNOSIS_CNF_DRIVER_H
#define DIAGNOSIS_CNF_DRIVER_H
#include <vector>
#include <map>
#include <string>
#include "lua.hpp"
namespace DIAGNOSIS {
    class CNFDriver {


    public:
        explicit CNFDriver(const std::string & fpath);
        ~CNFDriver();
        void newClause(int size);

        int addInputAtom(std::string &name);

        int addOutputAtom(std::string &name);

        int addWireAtom(std::string &name);

        void insertClauseAtom(int index, int atom);
        int addCompAtom(std::string &name);
        int getAtomInt(std::string &name);

        void print(std::ostream &stream);
        void parseRule(std::string &comp, std::string&name,std::vector<std::string>&params);

    protected:
        struct array {
            int *value;
            int size;
        };
        struct clauses {
            struct array *value;
            struct clauses *next;
        };
        enum Type {
            INPUT, OUTPUT, WIRE,COMP
        };

        struct atom {
            Type type;
            int index;
        };

        int addAtom(std::string &name, Type type);

        struct clauses *cnf_head{};
        struct clauses *cnf_tail{};
        std::map<std::string, atom *> _map;
    private:
        char  function_name[255]="comp_";
        lua_State *L;
        struct array *clause = NULL;
        int clause_size=0;
        int last_atom = 0;

        void _print(std::ostream &stream, Type type);
    };

}


#endif //DIAGNOSIS_CNF_DRIVER_H
