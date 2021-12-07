
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <ranges>

template<class Proj = std::identity>
auto getFuelCost(const auto alignPos, const auto& crabPositions, Proj proj = {}){
    auto oneCrabfuelCost = [alignPos, &proj](const auto crabPos){ return proj(std::abs(alignPos-crabPos)); };
    return Utilities::sum(crabPositions, 0u, oneCrabfuelCost);
}

auto getCheapestFuelCost1 = [](auto& crabPositions){
    const auto middleIt = std::begin(crabPositions)+crabPositions.size()/2;
    std::ranges::nth_element(crabPositions, middleIt);
    const auto median = *middleIt;
    return getFuelCost(median, crabPositions);
};

auto getCheapestFuelCost2 = [](const auto& crabPositions){
    auto leastFuel=UINT_MAX;
    for(auto alignPos = 0; alignPos < std::ranges::max(crabPositions); alignPos++){
        const auto fuel = getFuelCost(alignPos, crabPositions, [](const auto n){return n*(n+1)/2;});
        if(fuel < leastFuel) leastFuel = fuel;
    }
    return leastFuel;
};

int main(){
    auto crabPositions = readFile::vectorOfInts("input.txt", ',');

    //Task 1
    const auto cheapestFuelCost = getCheapestFuelCost1(crabPositions);
    std::cout << "To align the crabs, they need at least " << cheapestFuelCost << " amount of fuel.\n";

    //Task 2
    const auto cheapestFuelCost2 = getCheapestFuelCost2(crabPositions);
    std::cout << "With the updated crab understanding, they need at least " << cheapestFuelCost2 << " amount of fuel.\n";
}