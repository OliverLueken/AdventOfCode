
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

auto parseInput = [](const auto& input){
    std::vector<std::pair<int,int>> parameter{};
    auto i=1;
    for(const auto& row : input){
        const auto split = Utilities::split(row);
        const auto n = std::stoi(split[3]);
        const auto p = std::stoi(split[11]);
        const auto b = (n-i-p)%n;
        parameter.emplace_back(n, b);
        i++;
    }
    return parameter;
};


int main(){
    auto parameter = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto timeToDrop = Utilities::solveCRM(parameter);
    std::cout << "The time to drop the ball is at t=" <<timeToDrop <<  "\n";

    //Task 2
    auto n = 11;
    auto p = 0;
    auto b = (n-parameter.size()-1-p)%n;
    parameter.emplace_back(n, b);
    const auto timeToDrop2 = Utilities::solveCRM(parameter);
    std::cout << "The time to drop the ball is at t=" << timeToDrop2 <<  "\n";

    VerifySolution::verifySolution(timeToDrop, timeToDrop2);
}