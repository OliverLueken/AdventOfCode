
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>


auto solveRec(const auto first, const auto last, const auto desiredWeight,
           const auto elementCount, const auto weight, const auto qe,
           auto& minElements, auto& minQE){
    if(weight > desiredWeight || elementCount > minElements) return;

    if(weight == desiredWeight){
        if(elementCount < minElements){
            minElements = elementCount;
            minQE = qe;
        }
        else if(qe < minQE){
            minQE = qe;
        }
        return;
    }

    for(auto next=first+1; next!=last; next++){
        solveRec(next, last, desiredWeight, elementCount+1, weight+(*next), qe*(*next), minElements, minQE);
    }
}

auto getMinQuantumEntanglement = [](auto& weights, int groups){
    std::ranges::reverse(weights);

    const auto sum = std::accumulate(std::begin(weights), std::end(weights), 0u);
    const auto desiredWeight = sum/groups;
    auto minElements = weights.size();
    auto minQE = ULONG_MAX;

    for(auto first=std::begin(weights); first!=std::end(weights); first++){
        const auto elementCount = 1u;
        const auto weight = (unsigned int) *first;
        const auto qe = (unsigned long) *first;
        solveRec(first, std::end(weights), desiredWeight,
                 elementCount, weight, qe, minElements, minQE);
    }

    return minQE;
};

int main(){
    auto weights = readFile::vectorOfInts("input.txt");

    //Task 1
    auto minimalQuantumEntaglement = getMinQuantumEntanglement(weights, 3);
    std::cout << "The minimal quantum entanglement is " << minimalQuantumEntaglement << ".\n";

    //Task 2
    minimalQuantumEntaglement = getMinQuantumEntanglement(weights, 4);
    std::cout << "With four trunks, the minimal quantum entanglement is " << minimalQuantumEntaglement << ".\n";
}