//
// Created by Randy Collado on 2/14/22.
//

#include "TPHQueryGenerator.h"

int main(int argc, char** argv){

    TPHQueryGenerator gen(std::vector<int>{5, 8, 20});
    gen.runQueries(10);
    gen.outputStatistics(true);
    return 0;
}

