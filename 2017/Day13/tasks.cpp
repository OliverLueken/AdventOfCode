
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>

struct Layer{
    int depth;
    int range;
};

auto parseInput = [](const auto& input){
    std::vector<Layer> firewall;
    for(const auto& row : input){
        auto split = Utilities::split(row);
        firewall.emplace_back(std::stoi(split[0]), std::stoi(split[1]));
    }
    return firewall;
};

auto getSeverity(const auto& firewall, int startTime){
    auto severity = 0;
    for(const auto& layer : firewall){
        const int time = startTime+layer.depth;
        if(time%(2*layer.range-2) == 0){
            severity+=layer.depth*layer.range;
        }
    }
    return severity;
}

auto getResult = [](const auto& firewall){
    return getSeverity(firewall, 0);
};

auto getResult2 = [](const auto& firewall){

    return 0;
};

int main(){
    const auto firewall = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result = getResult(firewall);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(firewall);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}