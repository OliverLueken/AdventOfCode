
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <ranges>
#include <cmath>

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

<<<<<<< HEAD
auto getCheapestFuelCost2 = [](const auto& crabPositions){
=======
auto getCheapestFuelCost2 = []( auto crabPositions){
>>>>>>> 7f30c3d80f476a1fec2e7fd17ad730fa21953f46
    const auto mean = Utilities::sum(crabPositions, 0.)/(double)crabPositions.size();
    const auto fuelFloor = getFuelCost((int) std::floor(mean), crabPositions, [](const auto n){return n*(n+1)/2;});
    const auto fuelCeil  = getFuelCost((int) std::ceil (mean), crabPositions, [](const auto n){return n*(n+1)/2;});
    return std::min(fuelFloor, fuelCeil);
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