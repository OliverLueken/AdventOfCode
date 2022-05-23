
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

auto getResult = [](const auto& connections){
    auto groupCount = 0;
    auto groupSizeWithIdZero = 0ul;
    auto remainingIds = std::set<int>{};
    std::ranges::copy(connections | std::views::keys, std::inserter(remainingIds, std::begin(remainingIds)));
    while(!remainingIds.empty()){
        const auto id = *(remainingIds.begin());
        const auto group = getGroupWith(connections, id);

        if(group.contains(0)){
            groupSizeWithIdZero = group.size();
        }

        auto tempDifference = std::set<int>{};
        std::ranges::set_difference(remainingIds, group, std::inserter(tempDifference, std::begin(tempDifference)));
        std::swap(remainingIds, tempDifference);
        ++groupCount;
    }
    return std::make_pair(groupSizeWithIdZero, groupCount);
};

auto getResult2 = [](const auto& connections){

    return 0;
};

int main(){
    const auto connections = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto [result, result2] = getResult(connections);
    std::cout << "Task 1: " << result << ".\n";

    //Task 2
    std::cout << "Task 2: " << result2 << ".\n";

    VerifySolution::verifySolution(result, result2);
}