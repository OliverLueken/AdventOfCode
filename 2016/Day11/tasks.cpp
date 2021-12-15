
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <variant>

struct Building{
    std::array<std::vector<int>,4> floors{};
};

auto add = [](const auto& object, const auto& split, auto& floor, auto& typeIDs){
    static int id=0;
    auto it=std::ranges::find(split, object);
    while(it!=std::end(split)){
        auto thisID = -id;
        const auto specifier = Utilities::split(*(it-1), '-')[0];
        if(typeIDs.contains( specifier )){
            thisID = -typeIDs[ specifier ];
        }
        else{
            typeIDs[ specifier ] = id;
            id++;
        }
        floor.push_back(thisID);
        it=std::ranges::find(it+1, std::end(split), object);
    }
};

auto parseRow = [](const auto& s, auto& typeIDs){
    std::vector<int> floor{};
    const auto split = Utilities::splitOnEach(s, " ,.");
    add("generator", split, floor, typeIDs);
    add("microchip", split, floor, typeIDs);

    return floor;
};

auto parseInput = [](const auto& input){
    std::unordered_map<std::string, int> typeIDs{};
    Building building{};
    int floor=0u;
    for(const auto& s : input){
        building.floors[floor++] = parseRow(s, typeIDs);
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
    building.floors[0].push_back(8);
    building.floors[0].push_back(-8);
    building.floors[0].push_back(9);
    building.floors[0].push_back(-9);
    const auto minSteps2 = getMinSteps(building);
    std::cout << "The minimum number of steps " << minSteps2 << ".\n";
}