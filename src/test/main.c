#include <stdio.h>

#include "../../cutest-1.5/CuTest.h"


CuSuite *AppGetSuite();

CuSuite *MultisetGetSuite();

CuSuite *AvlTreeGetSuite();

CuSuite *Sha1GetSuite();

CuSuite *Md5GetSuite();

CuSuite *PolynomialGetSuite();

int RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, AppGetSuite());
    CuSuiteAddSuite(suite, MultisetGetSuite());
    CuSuiteAddSuite(suite, AvlTreeGetSuite());
    CuSuiteAddSuite(suite, Sha1GetSuite());
    CuSuiteAddSuite(suite, Md5GetSuite());
    CuSuiteAddSuite(suite, PolynomialGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);

    return !!suite->failCount;
}


int main(void) {
    return RunAllTests();
}
