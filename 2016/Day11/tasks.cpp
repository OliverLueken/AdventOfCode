
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <ranges>


using Configuration = std::unordered_map<int, int>;

class Building{
    std::unordered_map<std::string, int> typeIDs{};

    auto addSpecificObjects(const std::string& object, const auto& split, const auto floorNumber){
        auto it=std::ranges::find(split, object);
        while(it!=std::end(split)){
            //we need to separate the -compatible for the microchips to get their specification
            const auto specifier = Utilities::split(*(it-1), '-')[0];
            int thisID{};
            if(typeIDs.contains( specifier )){
                thisID = -typeIDs[ specifier ];
            }
            else{
                thisID = static_cast<int>(typeIDs.size())+1;
                if(object == "generator"){
                    thisID*=-1;
                }
                typeIDs[ specifier ] = thisID;
            }

            configuration.emplace(thisID, floorNumber);
            it=std::ranges::find(it+1, std::end(split), object);
        }
    };

public:
    Configuration configuration{};

    auto addObjects(const std::string& s, const auto floorNumber){
        const auto split = Utilities::splitOnEach(s, " ,.");
        addSpecificObjects("microchip", split, floorNumber);
        addSpecificObjects("generator", split, floorNumber);
    };

};


auto parseInput = [](const auto& input){
    Building building{};

    building.configuration.emplace(0,0); //Adds elevator to first floor
    for(auto floorNumber=0u; floorNumber<input.size(); floorNumber++){
        building.addObjects(input[floorNumber], floorNumber);
    }
    return building;
};


auto hash = [](const Configuration& configuration) {
    auto hashVal = 0ul;
    const std::array<unsigned long,4> offset = {1, 1'00'00, 1'00'00'00'00, 1'00'00'00'00'00'00};
    for(const auto& [id, floor] : configuration){
        if(id>0){
            hashVal+=offset[floor];
            hashVal+=offset[floor]*100*std::abs(configuration.at(-id)-floor);
        }
        else{
            hashVal+= id != 0 ? 10*offset[floor] : 0;
        }
    }
    hashVal<<=2;
    return hashVal+configuration.at(0);
};


auto getGoalConfiguration(const auto& configuration){
    Configuration goalConfiguration{};

    for(const auto& [id, floor] : configuration){
        goalConfiguration.emplace(id, 3);
    }

    return goalConfiguration;
}

auto isValid(const auto& config){
    auto soloChips = [&config](const auto& pair){
        const auto& [id, floor] = pair;
        return id >0 && floor != config.at(-id);
    };
    std::unordered_set<int> floorsWithSoloChips{};
    for( const auto floor : config | std::views::filter(soloChips) | std::views::values){
        floorsWithSoloChips.insert(floor);
    }

    auto generators = [](const auto& pair){return pair.first<0;};
    auto equalWithFloorsWithSoloChips = [&floorsWithSoloChips](const auto generatorFloor){
        return floorsWithSoloChips.contains(generatorFloor);
    };

    auto floorsWithGenerators = config | std::views::filter(generators) | std::views::values;
    return std::ranges::none_of(floorsWithGenerators, equalWithFloorsWithSoloChips);
}

auto addNextConfigurationInDirection(const auto& configuration, const auto direction, auto& nextConfigurations, const auto id1, const auto id2, auto& configurationsReached){
    Configuration newConfig{configuration};
    newConfig[0] = configuration.at(0)+direction;
    newConfig[id1]+=direction;
    if(id1 != id2){
        newConfig[id2]+=direction;
    }

    const auto [_, inserted] = configurationsReached.insert(hash(newConfig));

    if(inserted && isValid(newConfig)){
        nextConfigurations.emplace_back( std::move(newConfig) );
    }
}

auto addAllNextConfigurationsInDirection(const auto& configuration, const auto direction, auto& nextConfigurations, auto& configurationsReached){
    const auto elevatorFloor = configuration.at(0);
    auto onElevatorFloor = [&elevatorFloor](const auto& pair){
        return pair.second==elevatorFloor && pair.first!=0;
    };
    auto IDsOnElevatorFloor = configuration | std::views::filter(onElevatorFloor) | std::views::keys;
    for(const auto& id1 : IDsOnElevatorFloor){
        for(const auto& id2 : IDsOnElevatorFloor){
            addNextConfigurationInDirection(configuration, direction, nextConfigurations, id1, id2, configurationsReached);
        }
    }
}

auto addNextConfigurations(const auto& configuration, auto &nextConfigurations, auto& configurationsReached){
    const auto elevatorFloor = configuration.at(0);

    if(elevatorFloor<3) addAllNextConfigurationsInDirection(configuration,  1, nextConfigurations, configurationsReached);
    if(elevatorFloor>0) addAllNextConfigurationsInDirection(configuration, -1, nextConfigurations, configurationsReached);

}

auto getMinSteps = [](const auto& building){
    const auto& startConfiguration = building.configuration;
    std::unordered_set<size_t> configurationsReached{hash(startConfiguration)};

    const auto goalConfiguration = getGoalConfiguration(startConfiguration);
    const auto goalHash = hash(goalConfiguration);

    std::vector<Configuration> currentConfigurations{};
    std::vector<Configuration> nextConfigurations{startConfiguration};

    auto currentStep = 0u;
    while(!nextConfigurations.empty()){
        std::swap(currentConfigurations, nextConfigurations);
        for(const auto& currentConfiguration : currentConfigurations){
            const auto currentHash = hash(currentConfiguration);
            if(currentHash == goalHash){
                return currentStep;
            }
            addNextConfigurations(currentConfiguration, nextConfigurations, configurationsReached);
        }
        currentConfigurations.clear();
        currentStep++;
    }
    return currentStep;
};

int main(){
    auto building = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto minSteps = getMinSteps(building);
    std::cout << "The minimum number of steps " << minSteps << ".\n";

    //Task 2
    building.addObjects("An elerium generator.",             0);
    building.addObjects("An elerium-compatible microchip.",  0);
    building.addObjects("A dilithium generator.",            0);
    building.addObjects("A dilithium-compatible microchip.", 0);

    const auto minSteps2 = getMinSteps(building);
    std::cout << "The minimum number of steps " << minSteps2 << ".\n";

    VerifySolution::verifySolution(minSteps, minSteps2);
}