
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>

auto parseInput(auto&& input){
    input.push_back(0);
    std::ranges::sort(input);
    return std::move(input);
}

auto possibleArrangementsOfNConsecutiveAdapters(const auto n){
    return (1<<std::max(n-1,0)) - (1<<std::max(n-3,0)) + 1;
}

auto getResults(const auto& adapters){
    auto totalPossibleArrangements = 1ul;
    std::unordered_map<int, int> differenceCounts{};
    auto consecutiveAdapterCount = 0;
    for(auto i=0u; i<adapters.size()-1; i++){
        const auto difference = adapters[i+1]-adapters[i];
        differenceCounts[difference]++;
        switch(difference){
        break; case 1:
            consecutiveAdapterCount++;
        break; case 3:
            totalPossibleArrangements*=possibleArrangementsOfNConsecutiveAdapters(consecutiveAdapterCount);
            consecutiveAdapterCount = 0;
        }
    }

    differenceCounts[3]++;
    const auto productOfDifferenceCounts=differenceCounts[1]*differenceCounts[3];
    totalPossibleArrangements*=possibleArrangementsOfNConsecutiveAdapters(consecutiveAdapterCount);
    return std::make_pair(productOfDifferenceCounts, totalPossibleArrangements);
}

int main(){
    const auto adapters = parseInput(readFile::vectorOfInts());

    //Task 1
    const auto [productOfDifferenceCounts, totalPossibleArrangements] = getResults(adapters);
    std::cout << "The product of the two difference counts is " << productOfDifferenceCounts << ".\n";

    //Task 2
    std::cout << "There are a total of " << totalPossibleArrangements << " possible adapter arrangements.\n";

    VerifySolution::verifySolution(productOfDifferenceCounts, totalPossibleArrangements);
}