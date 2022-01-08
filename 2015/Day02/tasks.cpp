
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>

#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

auto getAmountOfPaperNeeded(const auto& input){

    auto getArea = [](const std::vector<int>& sideLengths){
        return sideLengths[0]*sideLengths[1]
                + 2*( sideLengths[0]*sideLengths[1]
                    + sideLengths[1]*sideLengths[2]
                    + sideLengths[2]*sideLengths[0]
        );
    };

    return Utilities::sum(input, 0, getArea);
}

auto getAmountOfRibbonNeeded(const auto& input){
    auto getRibbonLength = [](const std::vector<int>& sideLengths){
        return 2*(sideLengths[0]+sideLengths[1]) + sideLengths[0]*sideLengths[1]*sideLengths[2];
    };
    return Utilities::sum(input, 0u, getRibbonLength);
}

int main(){
    auto input = readFile::vectorOfVectorOfInts("input.txt", '\n', 'x');
    std::ranges::for_each(input, [](auto& sideLengths){ std::ranges::sort(sideLengths);});

    //Task 1
    const auto paperNeeded = getAmountOfPaperNeeded(input);
    std::cout << "The elves need " << paperNeeded << " square feet of wrapping paper.\n";

    //Task 2
    const auto ribbonNeeded = getAmountOfRibbonNeeded(input);
    std::cout << "The elves need " << ribbonNeeded << " feet of ribbon.\n";

    VerifySolution::verifySolution(paperNeeded, ribbonNeeded);
}
