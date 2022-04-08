
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>



auto getResult = [](const auto& parsedInput){
    auto sum = 0;
    for(const auto& row : parsedInput){
        const auto [min, max] = std::ranges::minmax(row);
        sum += max-min;
    }
    return sum;
};

auto getResult2 = [](const auto& parsedInput){

    return 0;
};

int main(){
    const auto parsedInput = readFile::vectorOfVectorOfInts("input.txt", '\n', '\t');

    //Task 1
    const auto result = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(parsedInput);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}