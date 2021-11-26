
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cassert>

auto parseInput = [](const auto&& input){
    std::vector<std::vector<int>> triangles{};
    for(const auto& row : input){
        auto split = Utilities::split(row);
        std::vector<int> triangle{};
        std::ranges::transform(split, std::back_inserter(triangle),
            [](const auto& s){return std::stoi(s);});
        triangles.push_back(triangle);
    }
    return triangles;
};

auto isValidTriangle = [](auto triangle){
    std::ranges::sort(triangle);
    return triangle[0]+triangle[1] > triangle[2];
};

auto countValidColTriangles = [](const auto& triangles){
    assert(((void)"Input file length must be divisible by 3.", triangles.size()%3==0));
    auto count = 0u;
    for(auto it=std::begin(triangles); it!=std::end(triangles); it+=3){
        for(auto i : std::views::iota(0,3)){
            if(isValidTriangle( std::vector<int>{it->at(i), (it+1)->at(i), (it+2)->at(i)} )) count++;
        }
    }
    return count;
};

int main(){
    const auto triangles = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    auto validTriangles = std::ranges::count_if(triangles, isValidTriangle);
    std::cout << "There are " << validTriangles << " possible triangles.\n";

    //Task 2
    validTriangles = countValidColTriangles(triangles);
    std::cout << "There are " << validTriangles << " possible triangles by column.\n";
}