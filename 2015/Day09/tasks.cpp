
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

auto fillDistanceMatrix = [](const auto& strings, const auto citiesCount) -> std::vector<std::vector<unsigned int>> {
    std::vector<std::vector<unsigned int>> distanceMatrix(citiesCount, std::vector<unsigned int>(citiesCount));
    for(auto i=0u; i<citiesCount; i++){
        for(auto j=i+1; j<citiesCount; j++){
            const auto stringsIndex = (2*citiesCount-i-3)*i/2+j-1;
            const auto split = Utilities::split(strings[stringsIndex]);
            distanceMatrix[j][i] = distanceMatrix[i][j] = std::stoi(split[4]);
        }
    }
    return distanceMatrix;
};

auto tsp = [](const auto& matrix, const auto citiesCount, const auto cmp) -> unsigned int {

    auto sumDistances = [&matrix, &citiesCount, &cmp](const auto& path) -> unsigned int{
        auto distance = matrix[path[citiesCount-2]][citiesCount-1]; //completes the loop from the last city in path to end/start city
        auto max = distance; //keep track of the longest distance in the loop to remove later
        auto lastCity = citiesCount-1; //start loop at end/start city
        for(const auto nextCity : path){
            distance += matrix[lastCity][nextCity];
            if( cmp(max, matrix[lastCity][nextCity]) ) max = matrix[lastCity][nextCity];
            lastCity = nextCity;
        }
        distance -= max;
        return distance;
    };

    std::vector<size_t> path(citiesCount-1);
    std::iota(std::begin(path), std::end(path), 0u);

    auto minDistance = sumDistances(path);

    while(std::ranges::next_permutation(path).found){
        const auto distance = sumDistances(path);
        if( cmp(distance,minDistance) ) minDistance = distance;
    }
    return minDistance;
};

int main(){
    const auto strings = readFile::vectorOfStrings("input.txt");

    const auto citiesCount = (size_t)(std::sqrt(1+8*strings.size())+1)/2;
    const auto distanceMatrix = fillDistanceMatrix(strings, citiesCount);

    //Task 1
    const auto minDistance = tsp(distanceMatrix, citiesCount, std::less<>());
    std::cout << "The shortest distance to visit every city amounts to " << minDistance << ".\n";

    //Task 2
    const auto maxDistance = tsp(distanceMatrix, citiesCount, std::greater<>());
    std::cout << "The greatest distance to visit every city amounts to " << maxDistance << ".\n";
}