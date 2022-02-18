//
// Created by Randy Collado on 2/14/22.
// Currently only times queries for MonetDB, fucntionality for PostgreSQL coming soon
//

#include "TPHQueryGenerator.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

//use this to input password for your cs561user psql user
//make sure the monetdb dbfarm is running before running test suite
#define PASSWORD 'password'

TPHQueryGenerator::TPHQueryGenerator(std::vector<std::string> queryFilepaths){
    commandSplits[0] = std::string{"mclient -dtask2database_monet <"};
    commandSplits[1] = std::string{">> out_monet.log"};
    commandSplits[2] = std::string{"export PGPASSWORD='password'; psql -U cs561user -d task2database_psql -f "};
    commandSplits[3] = std::string{">> out_psql.log"};

    queryfileNames = queryFilepaths;
    statistics = {
	    {"Q5_MONET_TOTAL", 0.0},
	    {"Q8_MONET_TOTAL", 0.0},
	    {"Q20_MONET_TOTAL", 0.0},
	    {"Q5_PSQL_TOTAL", 0.0},
	    {"Q8_PSQL_TOTAL", 0.0},
	    {"Q20_PSQL_TOTAL", 0.0},
	    {"NUM_RUNS", 0.0},
	    {"NUM_CUSTOM_RUNS", 0.0}


    };
    raw_psql_times = std::map<std::string, std::vector<double>>();
    raw_monet_times = std::map<std::string, std::vector<double>>();


}

TPHQueryGenerator::~TPHQueryGenerator(){

}
bool TPHQueryGenerator::runCustomQueries(unsigned int runs){
   std::vector<double> monet_times{};
   std::vector<double> psql_times{};
   for (unsigned int run = 1; run <= runs; run++){
        for (size_t i = 0; i < customfileNames.size(); ++i){
	    std::string query = customfileNames[i];
            std::string command = commandSplits[0] + query + commandSplits[1];
            auto start = std::chrono::high_resolution_clock::now();
            system(command.c_str());
            auto end = std::chrono::high_resolution_clock::now();
	    
	    std::chrono::duration<double> monet_duration = end - start;
	    command = commandSplits[2] + query + commandSplits[3];
	    start = std::chrono::high_resolution_clock::now();
	    system(command.c_str());
	    end = std::chrono::high_resolution_clock::now();

	    std::chrono::duration<double> psql_duration = end - start;
	    raw_monet_times[query].push_back(monet_duration.count());
	    raw_psql_times[query].push_back(psql_duration.count());
	    
	    if (statistics.find(query+"_MONET_TOTAL") == statistics.end()){
	    	statistics[query+"_MONET_TOTAL"] = monet_duration.count();
	    }
	    else {
	    	statistics[query+"_MONET_TOTAL"] += monet_duration.count();
	    }

	    if (statistics.find(query+"_PSQL_TOTAL") == statistics.end()){
	    	statistics[query+"_PSQL_TOTAL"] = psql_duration.count();
	    }
	    else {
	    	statistics[query+"_PSQL_TOTAL"] += psql_duration.count();
	    } 	 
	    	    
            std::cout << "Query: " << query << "\n" << "Time taken to execute query on MonetDB: " << monet_duration.count() << " s" << std::endl;
	    std::cout << "Query: " << query << "\n" << "Time taken to execute query on PostgreSQL: " << psql_duration.count() << " s" << std::endl;
	    std::cout << std::endl;
	}

	
    }
    statistics["NUM_CUSTOM_RUNS"] = runs;
    std::cout << "Number of runs: " << runs << std::endl;
    return true;
}

bool TPHQueryGenerator::addCustomQueries(const std::vector<std::string>& queries){
	customfileNames.insert(customfileNames.end(), queries.begin(), queries.end());
	return true;
 
}

bool TPHQueryGenerator::runQueries(unsigned int runs){
    std::vector<double> monet_times{};
    std::vector<double> psql_times{};
    for (unsigned int run = 1; run <= runs; run++){
        for (size_t i = 0; i < queryfileNames.size(); ++i){
	    std::string query = queryfileNames[i];
            std::string command = commandSplits[0] + query + commandSplits[1];
            auto start = std::chrono::high_resolution_clock::now();
            system(command.c_str());
            auto end = std::chrono::high_resolution_clock::now();
	    std::chrono::duration<double> monet_duration = end - start;
	    
	    monet_times.push_back(monet_duration.count());

	    command = commandSplits[2] + query + commandSplits[3];
	    start = std::chrono::high_resolution_clock::now();
	    system(command.c_str());
	    end = std::chrono::high_resolution_clock::now();

	    std::chrono::duration<double> psql_duration = end - start;
	    psql_times.push_back(psql_duration.count());

	    switch (i){
		    case 0:
			    statistics["Q5_MONET_TOTAL"] += monet_duration.count();
			    raw_monet_times["Q5"].push_back(monet_duration.count());
			    
			    statistics["Q5_PSQL_TOTAL"] += psql_duration.count();
			    raw_psql_times["Q5"].push_back(psql_duration.count());
			    break;
		    case 1:
			    statistics["Q8_MONET_TOTAL"] += monet_duration.count();
			    raw_monet_times["Q8"].push_back(monet_duration.count());
			    
			    statistics["Q8_PSQL_TOTAL"] += psql_duration.count();
			    raw_psql_times["Q8"].push_back(psql_duration.count());
			    break;
		    case 2:
			    statistics["Q20_MONET_TOTAL"] += monet_duration.count();
			    raw_monet_times["Q20"].push_back(monet_duration.count());
			    
			    statistics["Q20_PSQL_TOTAL"] += psql_duration.count();
			    raw_psql_times["Q20"].push_back(psql_duration.count());
			    break;
		    default:
			    break;
	    }
	    
            std::cout << "Query: " << query << "\n" << "Time taken to execute query on MonetDB: " << monet_duration.count() << " s" << std::endl;
            std::cout << "Query: " << query << "\n" << "Time taken to execute query PostgreSQL: " << psql_duration.count() << " s" << std::endl;
	    std::cout << std::endl;
	}
	statistics["NUM_RUNS"]++;
	
    }

    std::cout << "Number of runs: " << runs << std::endl;
    return true;
}

double TPHQueryGenerator::calcStandardDeviation(const std::vector<double>& data_points){
	double average = 0.0;
	double variance = 0.0;
	for (auto&& point : data_points){
		average += point;
	}
	average /= data_points.size();
	for (auto&& point : data_points){
		variance += ((point - average)*(point-average));
	}
	variance /= data_points.size();

	return sqrt(variance);
}

bool TPHQueryGenerator::outputStatistics(bool toLogFile){
	auto q5_monet_avg = statistics["Q5_MONET_TOTAL"] / statistics["NUM_RUNS"];
	auto q5_monet_std_dev = calcStandardDeviation(raw_monet_times["Q5"]);
	
	auto q8_monet_avg = statistics["Q8_MONET_TOTAL"] / statistics["NUM_RUNS"];
	auto q8_monet_std_dev = calcStandardDeviation(raw_monet_times["Q8"]);
	
	auto q20_monet_avg = statistics["Q20_MONET_TOTAL"] / statistics["NUM_RUNS"];
	auto q20_monet_std_dev = calcStandardDeviation(raw_monet_times["Q20"]);

	auto q5_psql_avg = statistics["Q5_PSQL_TOTAL"] / statistics["NUM_RUNS"];
	auto q5_psql_std_dev = calcStandardDeviation(raw_psql_times["Q5"]);
	
	auto q8_psql_avg = statistics["Q8_PSQL_TOTAL"] / statistics["NUM_RUNS"];
	auto q8_psql_std_dev = calcStandardDeviation(raw_psql_times["Q8"]);
	
	auto q20_psql_avg = statistics["Q20_PSQL_TOTAL"] / statistics["NUM_RUNS"];
	auto q20_psql_std_dev = calcStandardDeviation(raw_psql_times["Q20"]);

	std::cout << "Average execution time for query 5 on MonetDB: " << q5_monet_avg << " s" << std::endl;
	std::cout << "Standard deviation for query 5 execution time on MonetDB: " << q5_monet_std_dev << " s" << std::endl;
	std::cout << "Average execution time for query 5 on PostgreSQL: " << q5_psql_avg << " s" << std::endl;	
	std::cout << "Standard deviation for query 5 execution time on PostgreSQL: " << q5_psql_std_dev << " s" << std::endl;
	
	std::cout << std::endl;	
	std::cout << "Average execution time for query 8 on MonetDB: " << q8_monet_avg << " s" << std::endl;	
	std::cout << "Standard deviation for query 8 execution time on MonetDB: " << q8_monet_std_dev << " s" << std::endl;	
	std::cout << "Average execution time for query 8 on PostgreSQL: " << q8_psql_avg << " s" << std::endl;
	std::cout << "Standard deviation for query 8 execution time on PostgreSQl: " << q8_psql_std_dev << " s" << std::endl;
	
	std::cout << std::endl;	
	std::cout << "Average execution time for query 20 on MonetDB: " << q20_monet_avg << " s"  << std::endl;
	std::cout << "Standard deviation for query 20 execution time on MonetDB: " << q20_monet_std_dev << " s" << std::endl;
	std::cout << "Average execution time for query 20 on PostgreSQL: " << q20_psql_avg << " s" << std::endl;
	std::cout << "Standard deviation for query 20 execution time on PostgreSQL: " << q20_psql_std_dev << " s" << std::endl;
	std::cout << std::endl;
	for (auto&& customQuery : customfileNames){

		std::cout << "Average execution time for " << customQuery << " on MonetDB: " << statistics[customQuery+"_MONET_TOTAL"] / statistics["NUM_CUSTOM_RUNS"] << " s"  << std::endl;
		std::cout << "Standard deviation for " << customQuery << " execution time on MonetDB: " << calcStandardDeviation(raw_monet_times[customQuery]) << " s" << std::endl;
		std::cout << "Average execution time for " << customQuery << " on PostgreSQL: " << statistics[customQuery+"_PSQL_TOTAL"] / statistics["NUM_CUSTOM_RUNS"] << " s"  << std::endl;
		std::cout << "Standard deviation for " << customQuery << " execution time on PostgreSQL: " << calcStandardDeviation(raw_psql_times[customQuery]) << " s" << std::endl;
		std::cout << std::endl;
	}

	return true;
}


