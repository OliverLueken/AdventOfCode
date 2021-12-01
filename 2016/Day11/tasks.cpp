
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

struct Building{
    std::array<std::vector<int>,4> floors{};
};

auto addGenerator = [](const auto& split, auto& floor, auto& typeID, auto& id){
    auto it=std::ranges::find(split, "generator");
    while(it!=std::end(split)){
        auto thisID = -id;
        if(typeID.contains( *(it-1) )){
            thisID = -typeID[ *(it-1) ];
        }
        else{
            typeID[ *(it-1) ] = id;
            id++;
        }
        floor.push_back(thisID);
        it=std::ranges::find(it+1, std::end(split), "generator");
    }
};

auto addMicrochip = [](const auto& split, auto& floor, auto& typeID, auto& id){
    auto it=std::ranges::find(split, "microchip");
    while(it!=std::end(split)){
        auto thisID = id;
        if(typeID.contains( *(it-2) )){
            thisID = typeID[ *(it-2) ];
        }
        else{
            typeID[ *(it-2) ] = id;
            id++;
        }
        floor.push_back(thisID);
        it=std::ranges::find(it+1, std::end(split), "microchip");
    }
};

auto parseRow = [](const auto& s, auto& typeID, auto& id){
    std::vector<int> floor{};
    const auto split = Utilities::splitOnEach(s, " ,.-");
    Utilities::printVec(split);
    addGenerator(split, floor, typeID, id);
    addMicrochip(split, floor, typeID, id);

    return floor;
};

auto parseInput = [](const auto& input){
    std::unordered_map<std::string, int> typeID{};
    Building building{};
    int i=0;
    auto id=1;
    for(const auto& s : input){
        building.floors[i++] = parseRow(s, typeID, id);
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

    for(const auto& floor : building.floors){
        Utilities::printVec(floor);
    }

    //Task 1
    const auto minSteps = getMinSteps(building);
    std::cout << "The minimum number of steps " << minSteps << ".\n";

    //Task 2
    building.floors[0].push_back(8);
    building.floors[0].push_back(-8);
    building.floors[0].push_back(9);
    building.floors[0].push_back(-9);
    const auto minSteps2 = getMinSteps(building);
    std::cout << "The minimum number of steps " << minSteps2 << ".\n";
}