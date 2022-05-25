
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <deque>
#include <set>
#include <unordered_map>


using Connections = std::unordered_map<int, std::vector<int>>;

auto parseInput = [](const auto& input){
    auto connections = Connections{};
    for(const auto& row : input){
        const auto split = Utilities::splitOnEach(row, ", ");
        const auto id = std::stoi(split[0]);
        std::ranges::transform(split.begin()+2, split.end(), std::back_inserter(connections[id]), [](const auto& neighbor){
            return std::stoi(neighbor);
        });
    }
    return connections;
};

auto getGroupWith(const auto& connections, const auto id){
    auto group = std::set<int>{};
    auto toVisit = std::deque<int>{id};

    while(!toVisit.empty()){
        const auto nextId = toVisit.front();
        toVisit.pop_front();
        if(group.insert(nextId).second){
            std::ranges::copy(connections.at(nextId), std::back_inserter(toVisit));
        }
    }

    return group;
}

auto getConnectionGroupInformation = [](const auto& connections){
    auto groupSizeWithIdZero = 0ul;
    auto groupCount = 0;

    const auto allIds = connections | std::views::keys;
    auto remainingIds = std::set<int>{std::begin(allIds), std::end(allIds)};

    while(!remainingIds.empty()){
        const auto id = *(remainingIds.begin());
        const auto group = getGroupWith(connections, id);

        if(group.contains(0)){
            groupSizeWithIdZero = group.size();
        }
        ++groupCount;

        auto remainingIdsTemp = Utilities::setDifference(remainingIds, group);
        std::swap(remainingIds, remainingIdsTemp);
    }
    return std::make_pair(groupSizeWithIdZero, groupCount);
};

int main(){
    const auto connections = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto [groupSizeWithIdZero, groupCount] = getConnectionGroupInformation(connections);
    std::cout << "The group with programm 0 has a size of " << groupSizeWithIdZero << ".\n";

    //Task 2
    std::cout << "There are " << groupCount << " different groups.\n";

    VerifySolution::verifySolution(groupSizeWithIdZero, groupCount);
}