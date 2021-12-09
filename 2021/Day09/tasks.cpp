
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

auto parseInput = [](const auto& input){
    auto toInt = [](const auto c) -> int {return 0+c-'0';};
    const auto n = input.size();
    const auto m = input[0].size();
    Matrix::Matrix<int> matrix(n, m, input | std::views::join | std::views::transform(toInt));
    return matrix;
};

auto isLowPoint = [](const auto& heightmap, const auto longIndex){
    auto isHigher = [myHeight=heightmap[longIndex], &heightmap](const auto& neighborIndex){
        return myHeight < heightmap[neighborIndex];
    };
    return std::ranges::all_of(getNeighbors(heightmap, longIndex), isHigher);
};

auto getLowpoints = [](const auto& heightmap){
    std::vector<unsigned int> lowpoints{};
    for(auto longIndex=0u; longIndex<heightmap.data().size(); longIndex++){
        if( isLowPoint(heightmap, longIndex) ){
            lowpoints.push_back(longIndex);
        }
    }
    return lowpoints;
};

auto getSumOfRiskLevels = [](const auto& heightmap, const auto& lowpoints){
    return Utilities::sum(lowpoints, 0u, [&heightmap](const auto& lowpoint){
        return 1+heightmap[lowpoint];
    });
};

auto getBasinSize(auto& heightmap, const auto longIndex){
    if( heightmap[longIndex] == 9) return 0u;
    heightmap[longIndex] = 9;
    auto neighborsBasin = [&heightmap](const auto& neighborIndex){
        return getBasinSize(heightmap, neighborIndex);
    };
    return Utilities::sum( getNeighbors(heightmap, longIndex), 1u, neighborsBasin );
}

auto getLargestBasinsProduct = [](auto& heightmap, const auto& lowpoints){
    std::vector<unsigned int> basinSizes{};

    for(const auto longIndex : lowpoints){
        basinSizes.push_back( getBasinSize(heightmap, longIndex) );
    }
    std::ranges::partial_sort(basinSizes, std::begin(basinSizes)+3, std::greater<>());
    return basinSizes[0]*basinSizes[1]*basinSizes[2];
};

int main(){
    auto heightmap = parseInput(readFile::vectorOfStrings("input.txt"));
    const auto lowpoints = getLowpoints(heightmap);

    //Task 1
    const auto riskLevelSum = getSumOfRiskLevels(heightmap, lowpoints);
    std::cout << "The sum of the risk levels is " << riskLevelSum << ".\n";

    //Task 2
    const auto largestBasinsProduct = getLargestBasinsProduct(heightmap, lowpoints);
    std::cout << "The product of the three largest basins is " << largestBasinsProduct << ".\n";
}