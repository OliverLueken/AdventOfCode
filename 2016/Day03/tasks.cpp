
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cassert>
#include <array>

auto parseInput = [](const auto&& input){
    std::vector<std::array<unsigned int, 3>> triangles{};
    for(const auto& row : input){
        const auto split = Utilities::split(row);
        std::array<unsigned int, 3> triangle{};
        std::ranges::transform(split, std::begin(triangle),
            [](const auto& s){return std::stoi(s);});
        triangles.push_back(triangle);
    }
    return triangles;
};

auto isValidTriangle = [](const auto& triangle){
    const auto max = std::ranges::max(triangle);
    return Utilities::sum(triangle) > 2*max;
};

auto countValidColTriangles = [](const auto& triangles){
    assert(((void)"Input file length must be divisible by 3.", triangles.size()%3==0));
    auto count = 0u;
    for(auto i=0u; i<triangles.size()/3; i++){
        if( isValidTriangle( triangles | std::views::drop(3*i) | std::views::take(3) | std::views::elements<0> ) ) count++;
        if( isValidTriangle( triangles | std::views::drop(3*i) | std::views::take(3) | std::views::elements<1> ) ) count++;
        if( isValidTriangle( triangles | std::views::drop(3*i) | std::views::take(3) | std::views::elements<2> ) ) count++;
    }
    return count;
};

int main(){
    const auto triangles = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto validTriangles = std::ranges::count_if(triangles, isValidTriangle);
    std::cout << "There are " << validTriangles << " possible triangles.\n";

    //Task 2
    const auto validColumnTriangles = countValidColTriangles(triangles);
    std::cout << "There are " << validColumnTriangles << " possible triangles by column.\n";

    VerifySolution::verifySolution(validTriangles, validColumnTriangles);
}