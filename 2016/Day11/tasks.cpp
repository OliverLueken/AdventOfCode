
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

class Building{
    int id{1};
    std::unordered_map<std::string, int> typeIDs{};

    auto addSpecificObjects(const auto& object, const auto& split, const auto floorNumber){
        auto it=std::ranges::find(split, object);
        while(it!=std::end(split)){
            auto thisID = id;
            //we need to separate the -compatible for the microchips to get their specification
            const auto specifier = Utilities::split(*(it-1), '-')[0];
            if(typeIDs.contains( specifier )){
                thisID = -typeIDs[ specifier ];
            }
            else{
                typeIDs[ specifier ] = id;
                id++;
            }
            floors[floorNumber].push_back(thisID);
            it=std::ranges::find(it+1, std::end(split), object);
        }
    };

public:
    std::array<std::vector<int>,4> floors{};

    auto addObjects(const std::string& s, const auto floorNumber){
        const auto split = Utilities::splitOnEach(s, " ,.");
        addSpecificObjects("generator", split, floorNumber);
        addSpecificObjects("microchip", split, floorNumber);
    };

};


auto parseInput = [](const auto& input){
    Building building{};
    for(auto floorNumber=0u; floorNumber<input.size(); floorNumber++){
        building.addObjects(input[floorNumber], floorNumber);
    }
    return building;
};


auto getMinSteps = [](const auto& building){
    std::unordered_map<int, int> stepsFor{};
    for(auto floorIdx=0u; floorIdx<building.floors.size(); floorIdx++){
        const auto& floor=building.floors[floorIdx];
        for(auto i : floor){
            stepsFor[std::abs(i)]+=2*(3-floorIdx);
        }
    }
    auto r = stepsFor | std::views::values;
    return std::accumulate(std::begin(r), std::end(r), 3) - std::ranges::max(r);
};




int main(){
    auto building = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto minSteps = getMinSteps(building);
    std::cout << "The minimum number of steps " << minSteps << ".\n";

    //Task 2
    building.addObjects("An elerium generator.",             0u);
    building.addObjects("An elerium-compatible microchip.",  0u);
    building.addObjects("A dilithium generator.",            0u);
    building.addObjects("A dilithium-compatible microchip.", 0u);

    const auto minSteps2 = getMinSteps(building);
    std::cout << "The minimum number of steps " << minSteps2 << ".\n";
}