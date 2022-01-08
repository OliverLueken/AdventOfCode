
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Bounds{
    unsigned long left{0ul};
    unsigned long right{0ul};
};

auto parseInput = [](const auto& input){
    std::vector<Bounds> bounds{};
    std::ranges::transform(input, std::back_inserter(bounds),
        [](const auto& s){
            const auto split = Utilities::split(s, '-');
            return Bounds{std::stoul(split[0]), std::stoul(split[1])};
        }
    );
    std::ranges::sort(bounds,
        [](const auto& lhs, const auto& rhs){return lhs.left < rhs.left;}
    );
    return bounds;
};

auto getLowestValidIP = [](const auto& bounds, unsigned long searchStart = 0ul){
    auto lowestValidIP = searchStart;
    auto boundsIt = std::begin(bounds);
    while(lowestValidIP >= boundsIt->left){
        if(lowestValidIP <= boundsIt->right){
            lowestValidIP = boundsIt->right+1;
        }
        boundsIt++;
    }
    return lowestValidIP;
};

auto getValidIPsCount = [](const auto& bounds){
    auto validIPsCount = 0ul;
    auto currentIP = 0ul;
    for(const auto& bound : bounds){
        if(currentIP < bound.left){
            validIPsCount += bound.left-currentIP;
            currentIP = bound.right+1;
        }
        else if(currentIP <= bound.right){
            currentIP = bound.right+1;
        }
    }
    return validIPsCount;
};

int main(){
    const auto bounds = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto lowestValidIP = getLowestValidIP(bounds);
    std::cout << "The lowest valid IP is " << lowestValidIP << ".\n";

    //Task 2
    const auto validIPsCount = getValidIPsCount(bounds);
    std::cout << "The blacklist allows " << validIPsCount << " IPs.\n";

    VerifySolution::verifySolution(lowestValidIP, validIPsCount);
}