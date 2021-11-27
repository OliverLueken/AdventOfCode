
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

auto getNextRow = [](const auto& row){
    auto determineTile = [](const auto& it){ //it's a trap when left and right tiles are different
        return *(it-2) != *it ? '^' : '.';
    };

    std::string nextRow{'.'}; //left wall
    nextRow.reserve(row.size());
    for(auto it = std::begin(row)+2; it != std::end(row); it++){
        nextRow.push_back(determineTile(it));
    }
    nextRow.push_back('.'); // right wall
    return nextRow;
};

auto countSafeTiles = [](const auto& row){
    return std::count(std::begin(row)+1, std::end(row)-1, '.');
};

auto getNumberOfSafeTiles = [](auto row, unsigned int n){
    row.insert(std::begin(row), '.');
    row.push_back('.'); //adding walls
    auto safeTileCount = countSafeTiles(row);
    for(auto i=1u; i<n; i++){
        row = getNextRow(row);
        safeTileCount+=countSafeTiles(row);
    }
    return safeTileCount;
};

int main(){
    const auto input = readFile::string("input.txt");

    //Task 1
    const auto safeTileCount = getNumberOfSafeTiles(input, 40);
    std::cout << "There are " << safeTileCount << " safe tiles in 40 rows.\n";

    //Task 2
    const auto safeTileCount2 = getNumberOfSafeTiles(input, 400'000);
    std::cout << "There are " << safeTileCount2 << " safe tiles in 400,000 rows.\n";
}