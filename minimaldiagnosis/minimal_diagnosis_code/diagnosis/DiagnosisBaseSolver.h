//
// Created by zhangli on 2021/4/4.
//

#ifndef MINISAT_DIAGNOSISBASESOLVER_H
#define MINISAT_DIAGNOSISBASESOLVER_H

#include <minisat/simp/SimpSolver.h>
#include "../minimal/SpaceDefine.h"
class DiagnosisBaseSolver {
public:
    void readCNF(const char *path, bool strictp, int verbosity);

    virtual bool solve() = 0;

    inline int nVars() const {
        return solver.nVars();
    }
    inline void newVar() {
        solver.newVar();
    }

     SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &getModel() {
        return model;
    };

    static void printModel(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &model){
        printf("Model: ");
        for (int i = 0; i < model.size(); ++i) {
            if (model[i]==SOlVER_NAMESPACE::l_True)
                printf("%d ",i+1);
        }
        printf("\n");
    }

    virtual inline void addClause(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> &lits){
        if (lits.size()==0)return;
        solver.addClause_(lits);
        comp.clear();
        for (int i = 0; i < lits.size(); ++i) {
            comp.push(lits[i].x>>1);
        }
        Minisat::sort(comp);
    }
     inline void addClause_(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> &lits){}
    inline void getDiagnosis(SOlVER_NAMESPACE::vec<int> &diagnosis){
        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &model= getModel();
        for (int i = 0; i < comp.size(); ++i) {
            int lit= comp[i];
            if (model[lit]==SOlVER_NAMESPACE::l_True){
                diagnosis.push(lit);
            }
        }
    }

    virtual void printStats() { _printStats(); }

protected:
    void _printStats();
    double solve_time;
    double mini_time;
    SOlVER_NAMESPACE::vec<int> comp;
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> model;
    SOlVER_NAMESPACE::SimpSolver solver;
private:
    int _var=0;
};


#endif //MINISAT_DIAGNOSISBASESOLVER_H
