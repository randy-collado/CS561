//
// Created by Randy Collado on 2/14/22.
//

#include <iostream>
#include <map>
#include <vector>
#include <chrono>

#ifndef PROJECT_1_TPHQUERYGENERATOR_H
#define PROJECT_1_TPHQUERYGENERATOR_H

#define millisecond std::chrono::duration<double, std::milli> 

class TPHQueryGenerator {
    private:
        std::string commandSplits[2];
        std::vector<std::string> queryfileNames;
	std::map<std::string, millisecond> statistics;

    public:
        TPHQueryGenerator(std::vector<int> queryIDs);
        ~TPHQueryGenerator();

        bool runQueries(unsigned int runs);
        bool outputStatistics(bool toLogFile);
        std::vector<std::string> generateQueryStrings(std::vector<int> queryIDs);


};


#endif //PROJECT_1_TPHQUERYGENERATOR_H
