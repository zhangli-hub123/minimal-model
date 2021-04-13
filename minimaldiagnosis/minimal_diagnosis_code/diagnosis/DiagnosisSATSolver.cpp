//
// Created by zhangli on 2021/4/4.
//

#include "DiagnosisSATSolver.h"
#include "../minisat/mtl/Alg.h"
DiagnosisSATSolver::DiagnosisSATSolver() {

}


bool DiagnosisSATSolver::solve() {
    double start_time =SOlVER_NAMESPACE::cpuTime();
    bool sat=false;
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> litsT;
    if(compute_model_count>0){
        return result;
    }
    compute_model_count=1;
    solver.simplify();
    result = solver.solve(false, true);
    sat =result;
    double start_mini_time =SOlVER_NAMESPACE::cpuTime();
    while (sat){
        // TODO

        litsT.clear();
        model.clear();
        solver.model.copyTo(model);
//        printModel(model);
//        printf("Diagnosis: ");
        for (int i = 0; i <comp.size(); i++) {
            int lit =comp[i];
            SOlVER_NAMESPACE::lbool value=model[lit];
            if (value == SOlVER_NAMESPACE::l_True) {
//                printf("%d ",lit+1);
                litsT.push(SOlVER_NAMESPACE::mkLit(lit,true));
            } else{
                solver.addClause(SOlVER_NAMESPACE::mkLit(lit,true));
            }
        }
//        printf("\n\n");
        solver.addClause_(litsT);
        solver.simplify();
        ++compute_model_count;
        sat= solver.solve(false, true);
    }
        double end=SOlVER_NAMESPACE::cpuTime();
    solve_time = end -start_time;
    mini_time = end - start_mini_time;
    return result;
}