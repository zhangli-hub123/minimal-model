//
// Created by zhangli on 2020/11/26.
//

#include "cnf_driver.h"
#include <cstdio>
#include <iostream>
#include <cstring>
void DIAGNOSIS::CNFDriver::newClause(int size) {
    clause = (struct array *) malloc(sizeof(array));
    clause->value=new int[size];
    clause->size=size;
    ++clause_size;
    clauses *cls = (clauses *) malloc(sizeof(clauses));
    cls->value = clause;
    cls->next= nullptr;
    if (cnf_head == nullptr) {
        cnf_head = cls;
        cnf_tail = cnf_head;
    } else {
        cnf_tail->next=cls;
        cnf_tail=cls;
    }

}

int DIAGNOSIS::CNFDriver::addInputAtom(std::string &name) {
    return addAtom(name,INPUT);
}

int DIAGNOSIS::CNFDriver::addOutputAtom(std::string &name) {
    return addAtom(name,OUTPUT);

}

int DIAGNOSIS::CNFDriver::addWireAtom(std::string &name) {
    return addAtom(name,WIRE);
}


int DIAGNOSIS::CNFDriver::addAtom(std::string &name, DIAGNOSIS::CNFDriver::Type type) {
    auto tmp=_map.find(name);
    if (tmp!=_map.end()){
        return tmp->second->index;
    }
    atom * a = (atom*)malloc(sizeof(atom));
    a->type=type;
    a->index=++last_atom;
    _map.emplace(name,a);
    return last_atom;
}

void DIAGNOSIS::CNFDriver::insertClauseAtom(int index,int atom) {
    if (clause->size>index){
        clause->value[index]=atom;
    }
}

int DIAGNOSIS::CNFDriver::getAtomInt(std::string &name) {
    auto iter= _map.find(name);
    if (iter==_map.end()){
        return addWireAtom(name);
    }
    return iter->second->index;
}

void DIAGNOSIS::CNFDriver::print(std::ostream &stream) {
    // print cnf
    stream<<"c gen by circiut2cnf\n";
    stream<<"p cnf "<< last_atom<<" "<<clause_size<<"\n";
    clauses* iter= cnf_head;
    while (iter!= nullptr){
        array *arr=iter->value;
        int * ias= arr->value;
        for (int i = 0; i < arr->size; ++i) {
            stream<<ias[i] << " ";
        }
        stream << 0 <<"\n";
        iter=iter->next;
    }
    //print input;
    stream<< "I\n";
    _print(stream,INPUT);
    stream<< "O\n";
    _print(stream,OUTPUT);
    stream<< "W\n";
    _print(stream,WIRE);
    stream<< "C\n";
    _print(stream,COMP);
    stream.flush();
    

}

void DIAGNOSIS::CNFDriver::_print(std::ostream &stream,DIAGNOSIS::CNFDriver::Type type) {
    for (auto & iter : _map) {
        atom*a= iter.second;
        if (a->type==type){
            stream << iter.first << " "<< a->index << "\n";
        }
    }
}

DIAGNOSIS::CNFDriver::CNFDriver(const std::string &fpath) {

    L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, fpath.c_str()) != LUA_OK) {
        std::cerr<<"lua error: load file error"<<std::endl;
        exit(-1);
    }
    if (lua_pcall(L, 0, -1, 0) != LUA_OK) {
        std::cerr<<"lua error: load file error"<<std::endl;
        exit(-1);
    }
}

void DIAGNOSIS::CNFDriver::parseRule(std::string &comp, std::string &name, std::vector<std::string> &params) {
    strcpy(this->function_name+5,comp.c_str());
    lua_settop(L, 0);
    int r = lua_getglobal(L, this->function_name);
    if (r != LUA_TFUNCTION) {
        std::cerr << "lua error:" <<this->function_name << " is not function "<<std::endl;
        exit(-1);
    }
    lua_pushinteger(L, addCompAtom(name));
    lua_newtable(L);
    int index=0;
    for (auto & p:params){
        lua_pushinteger(L, ++index);
        lua_pushinteger(L, getAtomInt(p));
        lua_settable(L,-3);
    }

    if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
        const char *err = lua_tolstring(L, 1, NULL);
        std::cerr << "lua error:" <<err<<std::endl;
        exit(-1);
    }
    int s = lua_rawlen(L, -1);
    for (int i = 1; i < s + 1; ++i) {
        lua_pushinteger(L, i);
        if (lua_gettable(L, 1) != LUA_TTABLE) {
            lua_settop(L, -1);
            continue;
        }
        int size = lua_rawlen(L, -1);
        newClause(size);
        for (int j = 1; j < size + 1; ++j) {
            lua_pushinteger(L, j);
            if (lua_gettable(L, -2) != LUA_TNUMBER) {
                lua_settop(L, -2);
                continue;
            }
            int ok = lua_tointeger(L, -1);
            lua_remove(L, -1);
            insertClauseAtom(j-1,ok);
        }
        lua_settop(L, -1);
        lua_pushinteger(L, 1);
    }
}

int DIAGNOSIS::CNFDriver::addCompAtom(std::string &name) {
    return addAtom(name,COMP);
}

DIAGNOSIS::CNFDriver::~CNFDriver() {
    clauses* iter= cnf_head;
    while (iter!= nullptr){
        free(iter->value);
        clauses* tmp=iter;
        iter=iter->next;
        free(tmp);
    }
    cnf_head= nullptr;
    cnf_tail= nullptr;
}
