
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>


auto solveRec(const auto first, const auto last, const auto desiredWeight,
           const auto elementCount, const auto weight, const auto qe,
           auto& maxElements, auto& minQE){
    if(weight > desiredWeight || elementCount > maxElements) return;

    if(weight == desiredWeight){
        if(elementCount < maxElements){
            maxElements = elementCount;
            minQE = qe;
        }
        else if(qe < minQE){
            minQE = qe;
        }
        return;
    }

    for(auto next=first+1; next!=last; next++){
        solveRec(next, last, desiredWeight, elementCount+1, weight+(*next), qe*(*next), maxElements, minQE);
    }
}

auto getMinQuantumEntanglement = [](const auto& weights, int groups){
    const auto sum           = Utilities::sum(weights);
    const auto desiredWeight = sum/groups;
    const auto elementCount  = 1u;
    auto maxElements         = weights.size();
    auto minQE               = ULONG_MAX;

    for(auto first=std::begin(weights); first!=std::end(weights); first++){
        const auto weight = (unsigned int)  *first;
        const auto qe     = (unsigned long) *first;
        solveRec(first, std::end(weights), desiredWeight,
                 elementCount, weight, qe, maxElements, minQE);
    }

    return minQE;
};

int main(){
    auto weights = readFile::vectorOfInts("input.txt");
    std::ranges::reverse(weights);

    //Task 1
    const auto minimalQuantumEntaglement = getMinQuantumEntanglement(weights, 3);
    std::cout << "The minimal quantum entanglement is " << minimalQuantumEntaglement << ".\n";

    //Task 2
    const auto minimalQuantumEntaglementWithFourGroups = getMinQuantumEntanglement(weights, 4);
    std::cout << "With four trunks, the minimal quantum entanglement is " << minimalQuantumEntaglementWithFourGroups << ".\n";

    VerifySolution::verifySolution(minimalQuantumEntaglement, minimalQuantumEntaglementWithFourGroups);
}