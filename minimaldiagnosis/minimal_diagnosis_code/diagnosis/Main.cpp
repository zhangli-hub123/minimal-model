#include <errno.h>
#include <zlib.h>



#include <unordered_set>
#include <signal.h>
#include <minimal/MRSolver.h>
#include "../minimal/SpaceDefine.h"
#include "DiagnosisSATSolver.h"
#include "DiagnosisMinimalSolver.h"

static DiagnosisBaseSolver *solver;

static void SIGINT_exit(int) {
    printf("\n"); printf("*** INTERRUPTED ***\n");
   solver->printStats();
    printf("\n"); printf("*** INTERRUPTED ***\n");
    exit(1);
}
int main(int argc, char** argv) {
    try {
        SOlVER_NAMESPACE::setUsageHelp(
                "USAGE: %s [options] <input-file> <result-output-file>\n\n  where input may be either in plain or gzipped DIMACS.\n");
        SOlVER_NAMESPACE::setX86FPUPrecision();

        // Extra options:
        //
        SOlVER_NAMESPACE::IntOption verb("MAIN", "verb", "Verbosity level (0=silent, 1=some, 2=more).", 0,
                                         SOlVER_NAMESPACE::IntRange(0, 2));
        SOlVER_NAMESPACE::IntOption cpu_lim("MAIN", "cpu-lim", "Limit on CPU time allowed in seconds.\n", 0,
                                            SOlVER_NAMESPACE::IntRange(0, INT32_MAX));
        SOlVER_NAMESPACE::IntOption mem_lim("MAIN", "mem-lim", "Limit on memory usage in megabytes.\n", 0,
                                            SOlVER_NAMESPACE::IntRange(0, INT32_MAX));
        SOlVER_NAMESPACE::BoolOption strictp("MAIN", "strict", "Validate DIMACS header during parsing.", false);
        SOlVER_NAMESPACE::StringOption mod("MAIN", "mod", "Select a mod to compute mini diagnosis \n       SAT,MMSAT,MRSAT",
                                           "SAT");
        SOlVER_NAMESPACE::parseOptions(argc, argv, true);
        if (cpu_lim != 0) limitTime(cpu_lim);
        if (mem_lim != 0) limitMemory(mem_lim);
        SOlVER_NAMESPACE::sigTerm(SIGINT_exit);
        signal(SIGQUIT, SIGINT_exit);
        double initial_time = SOlVER_NAMESPACE::cpuTime();
        if (strcmp(&*mod,"SAT")==0){
            solver=new DiagnosisSATSolver();
        }
        if (strcmp(&*mod,"MMSAT")==0){
            solver=new DiagnosisMinimalSolver<Minimal::MMSolver>();
        }
        if (strcmp(&*mod,"MRSAT")==0){
            solver=new DiagnosisMinimalSolver<Minimal::MRSolver>();
        }
        if (solver== nullptr){
            fputs("please select mod from \" SAT,MMSAT,MRSAT \"",stderr);
            return -1;
        }
        printf("Mod：%s\n",&*mod);
        solver->readCNF(argv[1],strictp,verb);
        bool  result=solver->solve();
        solver->printStats();
        if(!result){
            printf("UNSATISFIABLE\n");
            return 0;
        }
        puts("SATISFIABLE");
        SOlVER_NAMESPACE::vec<int> diagnosis;
        printf("Diagnosis: ");
        solver->getDiagnosis(diagnosis);
        for (int i = 0; i < diagnosis.size(); ++i) {
            printf("%d ",diagnosis[i]+1);
        }
        printf("\nModel:");
        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &model= solver->getModel();
        for (int i = 0; i < model.size(); ++i) {
            if (model[i]==SOlVER_NAMESPACE::l_True)
            printf("%d ",i+1);
        }

        putc('\n',stdout);
    } catch (SOlVER_NAMESPACE::OutOfMemoryException &) {
        printf("===============================================================================\n");
        printf("INDETERMINATE\n");
        fflush(stdout);
        exit(0);
    }
    return 0;
}