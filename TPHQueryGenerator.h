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
        std::string commandSplits[4];
	//this vector holds the filepaths of the TPC-H benchmark queries
        std::vector<std::string> queryfileNames;
	//this vector holds the filepaths of any custom queries we may want to run
	std::vector<std::string> customfileNames;
	std::map<std::string, double> statistics;
	std::map<std::string, std::vector<double>> raw_monet_times;
	std::map<std::string, std::vector<double>> raw_psql_times;

	double calcStandardDeviation(const std::vector<double>& data_points);

    public:
        TPHQueryGenerator(std::vector<std::string> queryFilepaths);
        ~TPHQueryGenerator();

        bool runQueries(unsigned int runs);
	bool runCustomQueries(unsigned int runs);
        bool outputStatistics(bool toLogFile);
	bool addCustomQueries(const std::vector<std::string>& queries);
		


};


#endif //PROJECT_1_TPHQUERYGENERATOR_H
