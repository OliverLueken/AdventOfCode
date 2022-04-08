
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>

auto getInput(){
    auto input = readFile::vectorOfVectorOfInts("input.txt", '\n', '\t');
    std::ranges::for_each(input, [](auto& row){
        std::ranges::sort(row);
    });
    return input;
}

auto getSumOfMinMaxDifferences = [](const auto& spreadSheet){
    return Utilities::sum(spreadSheet, 0, [](const auto& row){
        return row.back()-row.front();
    });
};

auto getSumOfDivisiblePairDifferences = [](const auto& spreadSheet){

    auto getDivisiblePair = [](const auto& row){
        for(auto i=0u; i<row.size(); ++i){
            for(auto j=i+1; j<row.size(); ++j){
                if(row[j]%row[i]==0){
                    return std::make_pair(row[j], row[i]);
                }
            }
        }
        return std::make_pair(1,1);
    };

    return Utilities::sum(spreadSheet, 0, [getDivisiblePair](const auto& row){
        const auto [numerator, denominator] = getDivisiblePair(row);
        return numerator/denominator;
    });
};

int main(){
    const auto spreadSheet = getInput();

    //Task 1
    const auto sumOfMinMaxDifferences = getSumOfMinMaxDifferences(spreadSheet);
    std::cout << "The sum of the differences of between the max and min values of each row is " << sumOfMinMaxDifferences << ".\n";

    //Task 2
    const auto sumOfDivisiblePairDifferences = getSumOfDivisiblePairDifferences(spreadSheet);
    std::cout << "The sum of the differences between the two divisible pairs of each row is " << sumOfDivisiblePairDifferences << ".\n";

    VerifySolution::verifySolution(sumOfMinMaxDifferences, sumOfDivisiblePairDifferences);
}