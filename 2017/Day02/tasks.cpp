
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

auto getResult = [](const auto& parsedInput){
    return Utilities::sum(parsedInput, 0, [](const auto& row){
        return row.back()-row.front();
    });
};

auto getResult2 = [](const auto& parsedInput){

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

    return Utilities::sum(parsedInput, 0, [getDivisiblePair](const auto& row){
        const auto [numerator, denominator] = getDivisiblePair(row);
        return numerator/denominator;
    });
};

int main(){
    const auto parsedInput = getInput();

    //Task 1
    const auto result = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    //Task 2
    const auto result2 = getResult2(parsedInput);
    std::cout << "Task 2: " << result2 << ".\n";

    VerifySolution::verifySolution(result, result2);
}