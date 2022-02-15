//
// Created by Randy Collado on 2/14/22.
// Currently only times queries for MonetDB, fucntionality for PostgreSQL coming soon
//

#include "TPHQueryGenerator.h"
#include <iostream>
#include <vector>
#include <chrono>

TPHQueryGenerator::TPHQueryGenerator(std::vector<int> queryIDs){
    commandSplits[0] = std::string{"mclient -dtask2database_monet <"};
    commandSplits[1] = std::string{">> out.log"};

    queryfileNames = generateQueryStrings(queryIDs);
    statistics = {
	    {"Q5_TOTAL", 0.0},
	    {"Q8_TOTAL", 0.0},
	    {"Q20_TOTAL", 0.0},
	    {"NUM_RUNS", 0.0},


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
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	    if (!statistics.contains(query)){
	    	statistics[query] = duration.count();
	    }
	    else {
	    	statistics[query] += duration.count()
	    }
	    	    
            std::cout << "Query: " << query << "\n" << "Time taken to execute query: " << duration.count() << " ms" << std::endl;
	    std::cout << std::endl;
	}

	
    }
    statistics["NUM_CUSTOM_RUNS"] = runs;
    std::cout << "Number of runs: " << runs << std::endl;
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
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	    switch (i){
		    case 0:
			    statistics["Q5_TOTAL"] += duration.count();
			    break;
		    case 1:
			    statistics["Q8_TOTAL"] += duration.count();
			    break;
		    case 2:
			    statistics["Q20_TOTAL"] += duration.count();
			    break;
		    default:
			    break;
	    }
	    
            std::cout << "Query: " << query << "\n" << "Time taken to execute query: " << duration.count() << " ms" << std::endl;
	    std::cout << std::endl;
	}
	statistics["NUM_RUNS"]++;
	
    }
    std::cout << "Number of runs: " << runs << std::endl;
    return true;
}

bool TPHQueryGenerator::outputStatistics(bool toLogFile){
	auto q5_avg = statistics["Q5_TOTAL"] / statistics["NUM_RUNS"];
	auto q8_avg = statistics["Q8_TOTAL"] / statistics["NUM_RUNS"];
	auto q20_avg = statistics["Q20_TOTAL"] / statistics["NUM_RUNS"];

	std::cout << "Average timw for query 5: " << q5_avg << " ms" << std::endl;
	std::cout << "Average time for query 8: " << q8_avg << " ms" << std::endl;
	std::cout << "Average time for query 20: " << q20_avg << " ms"  << std::endl;

	return true;
}

std::vector<std::string> TPHQueryGenerator::generateQueryStrings(std::vector<int> queryIDs){
    std::vector<std::string> queryStrings;
    for(auto&& id : queryIDs){
        queryStrings.push_back("../queries/" + std::to_string(id) + ".sql");
    }
    return queryStrings;
}
