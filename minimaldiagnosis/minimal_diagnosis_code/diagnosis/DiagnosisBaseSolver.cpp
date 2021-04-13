//
// Created by zhangli on 2021/4/4.
//

#include "DiagnosisBaseSolver.h"
#include "../minimal/Dimacs.h"
void DiagnosisBaseSolver::_printStats() {
    double cpu_time =SOlVER_NAMESPACE::cpuTime();
    double mem_used =SOlVER_NAMESPACE::memUsedPeak();
    if (mem_used != 0) printf("Memory used           : %.2f MB\n", mem_used);
    if (solve_time != 0) printf("Solve time           : %g s\n", solve_time);
    if (mini_time != 0) printf("Minimal time           : %g s\n", mini_time);
    printf("CPU time              : %g s\n", cpu_time);
}

void DiagnosisBaseSolver::readCNF(const char *path, bool strictp, int verbosity) {
    gzFile in = gzopen(path, "rb");
    if (in == NULL)
        printf("ERROR! Could not open file: %s\n", path), exit(1);

    Minimal::parse_DIMACS(in, *this, this,(bool) strictp);
    gzclose(in);
}
