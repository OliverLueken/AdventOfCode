
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <ranges>


auto parseInput = [](const auto& input){
    Matrix::Matrix<int> octopiLightLevels{10, 10,
        input | std::views::join | std::views::transform([](const auto& c) ->int {return c-'0';})
    };
    return octopiLightLevels;
};

void increment(auto& octopiLightLevels){
    for(auto& octopusLightLevel : octopiLightLevels){
        octopusLightLevel++;
    }
}

auto flash(auto& octopiLightLevels){
    auto flashed = false;
    for(auto octopusIt = std::begin(octopiLightLevels); octopusIt!=std::end(octopiLightLevels); octopusIt++){
        auto& octopusLightLevel = *octopusIt;
        if(octopusLightLevel >= 10){
            flashed = true;
            octopusLightLevel=-20;

            auto neighbors = Matrix::getExtendedNeighbors(octopiLightLevels, octopusIt);
            for(const auto neighbor : neighbors){
                octopiLightLevels[neighbor]++;
            }
        }
    }
    return flashed;
}

auto reset(auto& octopiLightLevels){
    auto flashes = 0u;
    for(auto& octopusLightLevel : octopiLightLevels){
        if(octopusLightLevel<0){
            flashes++;
            octopusLightLevel=0;
        }
    }
    return flashes;
}

auto doOneStep(auto& octopiLightLevels){
    increment(octopiLightLevels);
    while(flash(octopiLightLevels));
    return reset(octopiLightLevels);
}

auto getFlashesIn100Steps = [](auto octopiLightLevels){
    auto flashes = 0u;
    for(auto step=0u; step<100; step++){
        flashes+=doOneStep(octopiLightLevels);
    }
    return flashes;
};

auto getStepWithBrightFlash = [](auto octopiLightLevels){
    for(auto step=1u;; step++){
        const auto flashes=doOneStep(octopiLightLevels);
        if(flashes == 100) return step;
    }
};

int main(){
    const auto octopiLightLevels = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto flashesIn100Steps = getFlashesIn100Steps(octopiLightLevels);
    std::cout << "There were a total of " << flashesIn100Steps << " flashes in 100 steps.\n";

    //Task 2
    const auto stepWithBrightFlash = getStepWithBrightFlash(octopiLightLevels);
    std::cout << "All octopi flash at the same time on step " << stepWithBrightFlash << ".\n";

    VerifySolution::verifySolution(flashesIn100Steps, stepWithBrightFlash);
}