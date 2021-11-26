
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>

#include "../../lib/readFile.hpp"

int main(){
    auto input = readFile::vectorOfVectorOfInts("input.txt", '\n', 'x');

    //Task 1
    auto getArea = [](std::vector<int>& sideLengths){
        std::ranges::sort(sideLengths);
        unsigned int area = sideLengths[0]*sideLengths[1]
                          + 2*( sideLengths[0]*sideLengths[1]
                              + sideLengths[1]*sideLengths[2]
                              + sideLengths[2]*sideLengths[0]
                              );
        return area;
    };

    unsigned int paperNeeded = std::transform_reduce(std::begin(input), std::end(input),
                                                     0,
                                                     std::plus<>(),
                                                     getArea);
    std::cout << "The elves need " << paperNeeded << " square feet of wrapping paper.\n";

    //Task 2
    auto getRibbonLength = [](const std::vector<int>& sideLengths){
        return 2*(sideLengths[0]+sideLengths[1]) + sideLengths[0]*sideLengths[1]*sideLengths[2];
    };
    unsigned int ribbonNeeded = std::transform_reduce(std::begin(input), std::end(input),
                                                      0,
                                                      std::plus<>(),
                                                      getRibbonLength);
    std::cout << "The elves need " << ribbonNeeded << " feet of ribbon.\n";
}
