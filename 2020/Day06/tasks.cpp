
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <ranges>

using Group = std::vector<std::string>;

auto parseInput = [](auto&& input){
    std::vector<Group> groups;
    Group group{};
    for(auto& row : input){
        if(row.empty()){
            groups.emplace_back(std::move(group));
            group.clear();
        }
        else{
            std::ranges::sort(row);
            group.emplace_back(std::move(row));
        }
    }
    groups.emplace_back(std::move(group));
    return groups;
};


auto getGroupCountSum = [](const auto& groups, const auto unionFunction){
    auto groupYesAnswerCount = [&unionFunction](const auto& group){
        auto binaryOp = [&unionFunction](const auto& lhs, const auto& rhs){
            std::string tempUnion{};
            unionFunction(lhs, rhs, std::back_inserter(tempUnion));
            return tempUnion;
        };
        return std::accumulate(std::begin(group)+1, std::end(group), group[0], binaryOp).size();
    };
    return Utilities::sum(groups, 0u, groupYesAnswerCount);
};

int main(){
    const auto groups = parseInput(readFile::vectorOfStrings("input.txt", '\n', true)); // true to keep empty rows to distinguish different groups

    //Task 1
    const auto sumOfYesAnswersAnyoneGave = getGroupCountSum(groups,std::ranges::set_union);
    std::cout << "The sum of yes answer anyone in a group gave is " << sumOfYesAnswersAnyoneGave << ".\n";

    //Task 2
    const auto sumOfYesAnswersEveryoneGave = getGroupCountSum(groups, std::ranges::set_intersection);
    std::cout << "The sum of yes answers everyone in a group gave is " << sumOfYesAnswersEveryoneGave << ".\n";

    VerifySolution::verifySolution(sumOfYesAnswersAnyoneGave, sumOfYesAnswersEveryoneGave);
}