
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
        const auto split = Utilities::split(row);
        firewall.emplace_back(std::stoi(split[0]), 2*std::stoi(split[1])-2);
    }
    return firewall;
};

auto getSeverity(const auto& firewall, const int startTime){
    auto severity = 0;
    for(const auto& layer : firewall){
        const int time = startTime+layer.depth;
        if(time%layer.range == 0){
            severity+=layer.depth*(layer.range+2)/2;
        }
    }
    return severity;
}

auto gotCaught(const auto& firewall, const int startTime){
    for(const auto& layer : firewall){
        const int time = startTime+layer.depth;
        if(time%layer.range == 0) return true;
    }
    return false;
}

auto getDelayToNotGetCaught = [](const auto& firewall){
    auto startTime = 0;
    while(gotCaught(firewall, startTime)){
        ++startTime;
    }
    return startTime;
};

int main(){
    const auto firewall = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto severityOfNotDelaying = getSeverity(firewall, 0);
    std::cout << "The severity of going through the firewall right away is " << severityOfNotDelaying << ".\n";

    //Task 2
    const auto minimumDelay = getDelayToNotGetCaught(firewall);
    std::cout << "We need to delay " << minimumDelay << " picoseconds to not get caught in the firewall.\n";

    VerifySolution::verifySolution(severityOfNotDelaying, minimumDelay);
}