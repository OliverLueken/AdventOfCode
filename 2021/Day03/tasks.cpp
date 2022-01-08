
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>

auto getColView = [](const auto& m, const auto col){
    auto nthElement = [&m, col](const auto row) -> char { return m[row][col]; };
    const auto colLength = m.size();
    return std::views::iota(0u, colLength) | std::views::transform(nthElement);
};

template<class Cmp = std::ranges::less_equal>
auto getMostCommonBit(const auto& report, const auto pos, Cmp cmp = {}){
    const auto numberOfOnes = std::ranges::count(getColView(report, pos), '1' );
    return cmp(report.size(), 2*numberOfOnes) ? '1' : '0';
}

auto binToInt(const std::string& bin){
    auto num = 0;
    for(const auto c : bin){
        num+=c-'0';
        num<<=1;
    }
    num>>=1;
    return num;
}

auto getPowerConsumption(const auto& report){
    const auto N = report[0].size();
    std::string mostCommonBits{};
    std::ranges::transform(std::views::iota(0u, N), std::back_inserter(mostCommonBits),
        [&report] (const auto pos){ return getMostCommonBit(report, pos); }
    );
    std::string invertedBits{};
    std::ranges::transform(mostCommonBits, std::back_inserter(invertedBits),
        [](const auto c){ return '0'+'1'-c;}
    );
    const auto gamma=binToInt(mostCommonBits);
    const auto eps  =binToInt(invertedBits);
    return gamma*eps;
}


template<class Cmp = std::ranges::less_equal>
auto getSupportRating(const auto& report, Cmp cmp = {}){
    auto rest{report};
    auto pos=0u;
    while(rest.size()>1){
        const auto bit = getMostCommonBit(rest, pos, cmp);
        std::erase_if(rest,
            [pos, bit](const auto number){ return number[pos]!=bit;}
        );
        pos++;
    }
    return binToInt(rest[0]);
}

auto getLifeSupportRating(const auto& report){
    const auto oxygenRating = getSupportRating(report);
    const auto CO2Rating    = getSupportRating(report, std::greater<>());
    return oxygenRating*CO2Rating;
}


int main(){
    const auto report = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto powerConsumption = getPowerConsumption(report);
    std::cout << "The submarine consumes " << powerConsumption << " amount of power.\n";

    //Task 2
    const auto lifeSupportRating = getLifeSupportRating(report);
    std::cout << "The life support rating of the submarine is at " << lifeSupportRating << ".\n";

    VerifySolution::verifySolution(powerConsumption, lifeSupportRating);
}