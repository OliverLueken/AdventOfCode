
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

enum class Direction{North=0, NorthEast, SouthEast, South, SouthWest, NorthWest};

Direction rotate(Direction d, int r){
    return static_cast<Direction>((static_cast<int>(d)+r)%6);
}

auto parseInput = [](const auto& input){
    const auto split = Utilities::split(input, ',');
    auto directions = std::vector<Direction>{};
    std::ranges::transform(split, std::back_inserter(directions), [](const auto& direction){
        if(direction == "n")  return Direction::North;
        if(direction == "ne") return Direction::NorthEast;
        if(direction == "se") return Direction::SouthEast;
        if(direction == "s")  return Direction::South;
        if(direction == "sw") return Direction::SouthWest;
        return Direction::NorthWest;
    });
    return directions;
};

auto getShortestDistance = [](const auto& directions){
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