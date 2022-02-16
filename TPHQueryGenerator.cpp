//
// Created by Randy Collado on 2/14/22.
// Currently only times queries for MonetDB, fucntionality for PostgreSQL coming soon
//

#include "TPHQueryGenerator.h"
#include <iostream>
#include <vector>
#include <chrono>

//use this to input password for your cs561user psql user
//make sure the monetdb dbfarm is running before running test suite
#define PASSWORD 'password'

TPHQueryGenerator::TPHQueryGenerator(std::vector<std::string> queryFilepaths){
    commandSplits[0] = std::string{"mclient -dtask2database_monet <"};
    commandSplits[1] = std::string{">> out_monet.log"};
    commandSplits[2] = std::string{"export PGPASSWORD='password'; psql -U cs561user -h 127.0.0.1 -d task2database_psql -f "};
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


}

TPHQueryGenerator::~TPHQueryGenerator(){

}
bool TPHQueryGenerator::runCustomQueries(unsigned int runs){
  for (unsigned int run = 1; run <= runs; run++){
        for (size_t i = 0; i < customfileNames.size(); ++i){
	    std::string query = customfileNames[i];
            std::string command = commandSplits[0] + query + commandSplits[1];
            auto start = std::chrono::high_resolution_clock::now();
            system(command.c_str());
            auto end = std::chrono::high_resolution_clock::now();
            auto monet_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	    command = commandSplits[2] + query + commandSplits[3];
	    start = std::chrono::high_resolution_clock::now();
	    system(command.c_str());
	    end = std::chrono::high_resolution_clock::now();
	    auto psql_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

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
	    	    
            std::cout << "Query: " << query << "\n" << "Time taken to execute query on MonetDB: " << monet_duration.count() << " ms" << std::endl;
	    std::cout << "Query: " << query << "\n" << "Time taken to execute query on PostgreSQL: " << psql_duration.count() << " ms" << std::endl;
	    std::cout << std::endl;
	}

	
    }
    statistics["NUM_CUSTOM_RUNS"] = runs;
    std::cout << "Number of runs: " << runs << std::endl;
    return true;
}

bool TPHQueryGenerator::addCustomQueries(std::vector<std::string> queries){
	customfileNames.insert(customfileNames.end(), queries.begin(), queries.end());
	return true;
 
}

bool TPHQueryGenerator::runQueries(unsigned int runs){
    for (unsigned int run = 1; run <= runs; run++){
        for (size_t i = 0; i < queryfileNames.size(); ++i){
	    std::string query = queryfileNames[i];
            std::string command = commandSplits[0] + query + commandSplits[1];
            auto start = std::chrono::high_resolution_clock::now();
            system(command.c_str());
            auto end = std::chrono::high_resolution_clock::now();
            auto monet_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	    command = commandSplits[2] + query + commandSplits[3];
	    start = std::chrono::high_resolution_clock::now();
	    system(command.c_str());
	    end = std::chrono::high_resolution_clock::now();
	    auto psql_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	    switch (i){
		    case 0:
			    statistics["Q5_MONET_TOTAL"] += monet_duration.count();
			    statistics["Q5_PSQL_TOTAL"] += psql_duration.count();
			    break;
		    case 1:
			    statistics["Q8_MONET_TOTAL"] += monet_duration.count();
			    statistics["Q8_PSQL_TOTAL"] += psql_duration.count();
			    break;
		    case 2:
			    statistics["Q20_MONET_TOTAL"] += monet_duration.count();
			    statistics["Q20_PSQL_TOTAL"] += psql_duration.count();
			    break;
		    default:
			    break;
	    }
	    
            std::cout << "Query: " << query << "\n" << "Time taken to execute query on MonetDB: " << monet_duration.count() << " ms" << std::endl;
            std::cout << "Query: " << query << "\n" << "Time taken to execute query PostgreSQL: " << psql_duration.count() << " ms" << std::endl;
	    std::cout << std::endl;
	}
	statistics["NUM_RUNS"]++;
	
    }
    std::cout << "Number of runs: " << runs << std::endl;
    return true;
}

bool TPHQueryGenerator::outputStatistics(bool toLogFile){
	auto q5_monet_avg = statistics["Q5_MONET_TOTAL"] / statistics["NUM_RUNS"];
	auto q8_monet_avg = statistics["Q8_MONET_TOTAL"] / statistics["NUM_RUNS"];
	auto q20_monet_avg = statistics["Q20_MONET_TOTAL"] / statistics["NUM_RUNS"];

	auto q5_psql_avg = statistics["Q5_PSQL_TOTAL"] / statistics["NUM_RUNS"];
	auto q8_psql_avg = statistics["Q8_PSQL_TOTAL"] / statistics["NUM_RUNS"];
	auto q20_psql_avg = statistics["Q20_PSQL_TOTAL"] / statistics["NUM_RUNS"];

	std::cout << "Average execution time for query 5 on MonetDB: " << q5_monet_avg << " ms" << std::endl;
	std::cout << "Average execution time for query 5 on PostgreSQL: " << q5_psql_avg << " ms" << std::endl;
	
	std::cout << "Average execution time for query 8 on MonetDB: " << q8_monet_avg << " ms" << std::endl;	
	std::cout << "Average execution time for query 8 on PostgreSQL: " << q8_psql_avg << " ms" << std::endl;
	
	std::cout << "Average execution time for query 20 on MonetDB: " << q20_monet_avg << " ms"  << std::endl;
	std::cout << "Average execution time for query 20 on PostgreSQL: " << q20_psql_avg << " ms" << std::endl;

	for (auto&& customQuery : customfileNames){

		std::cout << "Average execution time for " << customQuery << " on MonetDB: " << statistics[customQuery+"_MONET_TOTAL"] / statistics["NUM_CUSTOM_RUNS"] << " ms"  << std::endl;
		std::cout << "Average execution time for " << customQuery << " on PostgreSQL: " << statistics[customQuery+"_PSQL_TOTAL"] / statistics["NUM_CUSTOM_RUNS"] << " ms"  << std::endl;
	}

	return true;
}


