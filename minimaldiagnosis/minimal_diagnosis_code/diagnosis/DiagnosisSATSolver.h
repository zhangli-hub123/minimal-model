//
// Created by zhangli on 2021/4/4.
//

#ifndef MINISAT_DIAGNOSISSATSOLVER_H
#define MINISAT_DIAGNOSISSATSOLVER_H
#include "../minimal/BaseSolver.h"
#include "../minisat/mtl/Sort.h"
#include "DiagnosisBaseSolver.h"

class DiagnosisSATSolver: public DiagnosisBaseSolver {

public:
    DiagnosisSATSolver();
    bool solve();
    inline int nVars() const {
        return solver.nVars();
    }
    inline void newVar() {
        solver.newVar();
    }

private:
    int compute_model_count = 0;
    bool result = false;
};


#endif //MINISAT_DIAGNOSISSATSOLVER_H
