
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>

auto parseInput(auto&& input){
    input.push_back(0);
    std::ranges::sort(input);
    input.push_back(input.back()+3);
    std::vector<int> differences{};
    for(auto i=1u; i<input.size(); i++){
        differences.push_back( input[i]-input[i-1] );
    }
    return differences;
}

auto getProductOfDifferenceCounts(const auto& differences){
    std::unordered_map<int, int> differenceCounts{};
    for(const auto& difference : differences){
        differenceCounts[difference]++;
    }

    return differenceCounts[1]*differenceCounts[3];
}

auto possibleArrangementsOfNConsecutiveAdapters(const auto n){
    return (1<<std::max(n-1,0)) - (1<<std::max(n-3,0)) + 1;
}

auto getTotalPossibleArrangements(const auto& differences){
    auto totalPossibleArrangements = 1ul;
    auto consecutiveAdapterCount   = 0;
    for(const auto& difference : differences){
        switch(difference){
        break; case 1:
            consecutiveAdapterCount++;
        break; case 3:
            totalPossibleArrangements *= possibleArrangementsOfNConsecutiveAdapters(consecutiveAdapterCount);
            consecutiveAdapterCount    = 0;
        }
    }
    return totalPossibleArrangements;
}

int main(){
    const auto differences = parseInput(readFile::vectorOfInts());

    //Task 1
    const auto productOfDifferenceCounts = getProductOfDifferenceCounts(differences);
    std::cout << "The product of the two difference counts is " << productOfDifferenceCounts << ".\n";

    //Task 2
    const auto totalPossibleArrangements = getTotalPossibleArrangements(differences);
    std::cout << "There are a total of " << totalPossibleArrangements << " possible adapter arrangements.\n";

    VerifySolution::verifySolution(productOfDifferenceCounts, totalPossibleArrangements);
}