
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>


auto parseInput = [](const auto& input){
    std::unordered_map<std::string, std::vector<std::string>> caveConnectsTo{};

    auto addConnection = [&caveConnectsTo](const auto& currentCave, const auto& nextCave){
        if(currentCave != "end" && nextCave != "start"){
            caveConnectsTo[currentCave].push_back(nextCave);
        }
    };

    for(const auto& row : input){
        const auto split = Utilities::split(row, '-');
        const auto& lhs = split[0];
        const auto& rhs = split[1];
        addConnection(lhs, rhs);
        addConnection(rhs, lhs);
    }
    return caveConnectsTo;
};


auto visitNextCave(const std::string& currentCave, const auto& caveConnectsTo, auto& numberOfPaths,
                   auto& caveVisits, bool smallVisitedTwice){

    if(currentCave == "end"){
        numberOfPaths++;
        return;
    }

    if(std::islower(currentCave[0])){
        if(caveVisits[currentCave] >= 1 && smallVisitedTwice) return;

        if(caveVisits[currentCave] == 1){
            smallVisitedTwice=true;
        }
    }

    caveVisits[currentCave]++;
    for(const auto& nextCave : caveConnectsTo.at(currentCave) ){
        visitNextCave(nextCave, caveConnectsTo, numberOfPaths, caveVisits, smallVisitedTwice);
    }
    caveVisits[currentCave]--;
}

auto getNumberOfPathsThroughTheCave = [](const auto& caveConnectsTo, const bool canVisitOneSmallCaveTwice = false){
    auto numberOfPaths = 0u;
    std::unordered_map<std::string, int> caveVisits{};

    visitNextCave("start", caveConnectsTo, numberOfPaths, caveVisits, !canVisitOneSmallCaveTwice);

    return numberOfPaths;
};

int main(){
    const auto caveConnectsTo = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto numberOfPaths = getNumberOfPathsThroughTheCave(caveConnectsTo);
    std::cout << "There are " << numberOfPaths << " different paths through the cave.\n";

    //Task 2
    const auto numberOfPathsVisitingOneSmallCaveTwice = getNumberOfPathsThroughTheCave(caveConnectsTo, true);
    std::cout << "There are " << numberOfPathsVisitingOneSmallCaveTwice << " different paths through the cave while visiting a maximum of one small cave twice.\n";
}