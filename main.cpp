//
// Created by Randy Collado on 2/14/22.
//

#include "TPHQueryGenerator.h"

int main(int argc, char** argv){

    TPHQueryGenerator gen(std::vector<std::string>{"../queries/5.sql", "../queries/8.sql", "../queries/20.sql"});
    gen.runQueries(1);
    gen.addCustomQueries(std::vector<std::string>{"../custom/sum_lineitem.sql", "../custom/sum_max_lineitem.sql", "../custom/sum_max_max_lineitem.sql"});
    std::cout << "***Assigned Queries***" << std::endl;
    gen.runCustomQueries(1);
    gen.outputStatistics(true);
    return 0;
}

