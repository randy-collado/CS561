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
	//this vector holds the filepaths of the TPC-H benchmark queries
        std::vector<std::string> queryfileNames;
	//this vector holds the filepaths of any custom queries we may want to run
	std::vector<std::string> customfileNames;
	std::map<std::string, double> statistics;

    public:
        TPHQueryGenerator(std::vector<int> queryIDs);
        ~TPHQueryGenerator();

        bool runQueries(unsigned int runs);
        bool outputStatistics(bool toLogFile);
        std::vector<std::string> generateQueryStrings(std::vector<int> queryIDs);


};


#endif //PROJECT_1_TPHQUERYGENERATOR_H
