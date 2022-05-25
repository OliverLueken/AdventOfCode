
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>

struct Layer{
    int depth;
    int range;
};

auto parseInput = [](const auto& input){
    std::vector<Layer> parsed;
    for(const auto& row : input){
        auto split = Utilities::split(row);
        parsed.emplace_back(std::stoi(split[0]), std::stoi(split[1]));
    }
    return parsed;
};

auto getResult = [](const auto& parsedInput){

    return 0;
};

auto getResult2 = [](const auto& parsedInput){

    return 0;
};

int main(){
    const auto parsedInput = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(parsedInput);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}