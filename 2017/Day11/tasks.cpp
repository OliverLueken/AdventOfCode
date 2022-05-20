
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <ranges>

enum class Direction{North, NorthEast, SouthEast, South, SouthWest, NorthWest};

auto parseInput = [](const auto& input){
    return Utilities::split(input, ',');
};

auto getDistance = [](const auto& directions){
    
    return 0;
};

auto getResult2 = [](const auto& directions){

    return 0;
};

int main(){
    const auto directions = parseInput(readFile::string());

    //Task 1
    const auto shortestDistance = getShortestDistance(directions);
    std::cout << "Task 1: " << shortestDistance << ".\n";

    // //Task 2
    // const auto result2 = getResult2(directions);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(shortestDistance, result2);
}