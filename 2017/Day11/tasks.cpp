
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

auto getDistance(auto& directionCount){
    static const auto allDirections = std::vector<Direction>{
        Direction::North,
        Direction::NorthEast,
        Direction::SouthEast,
        Direction::South,
        Direction::SouthWest,
        Direction::NorthWest
    };

    auto removeOpposingDirections = [](auto& _directionCount){
        for(const auto& direction : allDirections | std::views::take(3)){
            const auto val = std::min(_directionCount[direction], _directionCount[rotate(direction, 3)]);
            _directionCount[direction]-=val;
            _directionCount[rotate(direction, 3)]-=val;
        }
    };

    auto straightenDirections = [](auto& _directionCount){
        for(const auto& direction : allDirections){
            const auto val = std::min(_directionCount[direction], _directionCount[rotate(direction, 2)]);
            _directionCount[direction]-=val;
            _directionCount[rotate(direction, 2)]-=val;
            _directionCount[rotate(direction, 1)]+=val;
        }
    };

    removeOpposingDirections(directionCount);
    straightenDirections(directionCount);
    removeOpposingDirections(directionCount);

    return Utilities::sum(directionCount | std::views::values);
}

auto getDistanceFromDestination = [](const auto& directions){
    std::unordered_map<Direction, int> directionCount{};
    std::ranges::for_each(directions, [&directionCount](const auto& direction){
        ++directionCount[direction];
    });
    return getDistance(directionCount);
};

auto getMaxDistance = [](const auto& directions){
    std::unordered_map<Direction, int> directionCount{};
    auto maxDistance = 0u;
    std::ranges::for_each(directions, [&directionCount, &maxDistance](const auto& direction){
        ++directionCount[direction];
        maxDistance = std::max(maxDistance, getDistance(directionCount));
    });
    return maxDistance;
};

int main(){
    const auto directions = parseInput(readFile::string());

    //Task 1
    const auto shortestDistance = getDistanceFromDestination(directions);
    std::cout << "Task 1: " << shortestDistance << ".\n";

    //Task 2
    const auto maxDistance = getMaxDistance(directions);
    std::cout << "Task 2: " << maxDistance << ".\n";

    VerifySolution::verifySolution(shortestDistance, maxDistance);
}