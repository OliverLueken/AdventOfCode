
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>


auto parseInput(const auto& input){
    const auto startTime = std::stoi(input[0]);
    const auto busIDs = Utilities::split(input[1], ',');
    std::vector<std::pair<int,int>> idOffsetPair{};
    for(auto i=0u; i<busIDs.size(); ++i){
        if(busIDs[i][0]=='x') continue;
        const auto id = std::stoi(busIDs[i]);
        idOffsetPair.emplace_back(id, id-i);
    }
    return std::make_pair(startTime, idOffsetPair);
}

auto getEarliestBusTimesWaitingTime(const auto startTime, const auto& ids) {
    const auto minIt = std::ranges::min_element( ids | std::views::elements<0>, std::less<>{},
        [startTime](const auto& id){
            return id-startTime%id;
        }
    );
    return *minIt * (*minIt-startTime%*minIt);
}

int main() {
    const auto [startTime, idOffsetPair] = parseInput(readFile::vectorOfStrings());

    // Task 1
    const auto earliestBusTimesWaitingtime = getEarliestBusTimesWaitingTime(startTime, idOffsetPair);
    std::cout << "The first bus ID times the minutes to wait is " << earliestBusTimesWaitingtime << ".\n";

    // Task 2
    const auto timeBusIDsAlign = Utilities::solveCRM(idOffsetPair);
    std::cout << "The earliest timesstamp the bus depart with an offset of their IDs is " << timeBusIDsAlign << ".\n";

    VerifySolution::verifySolution(earliestBusTimesWaitingtime, timeBusIDsAlign);
}