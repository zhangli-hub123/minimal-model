//
// Created by zhangli on 2021/4/4.
//

#ifndef MINISAT_DIAGNOSISMINIMALSOLVER_H
#define MINISAT_DIAGNOSISMINIMALSOLVER_H


#include "DiagnosisBaseSolver.h"
#include "../minimal/MMSolver.h"
template<class S>
class DiagnosisMinimalSolver : public DiagnosisBaseSolver{
public:
    bool solve();
    inline void addClause(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> &lits){
        DiagnosisBaseSolver::addClause(lits);
        SOlVER_NAMESPACE::sort(lits);
       s.addClause(lits);
    }
    // void copyToClauses(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &source, S &dist);
private:
    int compute_model_count = 0;
    bool result = false;
    S s;
    SOlVER_NAMESPACE::ClauseAllocator ca;
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> clauses;
};

template<class S>
bool DiagnosisMinimalSolver<S>::solve() {
    double start_time =SOlVER_NAMESPACE::cpuTime();
    bool sat=false;
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> litsT;
    result = s.solve();
    sat=result;
    double start_mini_time =SOlVER_NAMESPACE::cpuTime();
    model.clear();
    s.getModel().copyTo(model);
    while (sat){
        litsT.clear();
        for (int i = 0; i <comp.size(); i++) {
            int lit =comp[i];
            SOlVER_NAMESPACE::lbool value=model[lit];
            if (value == SOlVER_NAMESPACE::l_True) {
                litsT.push(SOlVER_NAMESPACE::mkLit(lit,true));
            } else{
                solver.addClause(SOlVER_NAMESPACE::mkLit(lit,true));
            }
        }
        solver.addClause_(litsT);
        ++compute_model_count;
        sat = solver.solve();
        model.clear();
        solver.model.copyTo(model);
    }
    double end=SOlVER_NAMESPACE::cpuTime();
    solve_time = end -start_time;
    mini_time = end - start_mini_time;
    return result;
}

// template<class S>
// void DiagnosisMinimalSolver<S>::copyToClauses(Minisat::vec<Minisat::CRef> &source, S &dist) {
//     SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> list;
//     while (nVars() >= dist.nVars()) dist.newVar();
//     for (int i = 0; i < source.size(); ++i) {
//         SOlVER_NAMESPACE::Clause &clause= ca[source[i]];
//         list.clear();
//         for (int j = 0; j < clause.size(); ++j) {
//             list.push(clause[j]);
//         }
//         dist.addClause(list);
//     }
// }

#endif //MINISAT_DIAGNOSISMINIMALSOLVER_H
