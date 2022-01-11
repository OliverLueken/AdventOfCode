
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <ranges>
#include <unordered_map>

auto parseInput = [](auto&& input){
    std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> bagRules;
    for(auto& row : input){
        const auto mainBagEndPos = row.find("bag");
        std::string mainBag{row, 0, mainBagEndPos-1};

        auto split = Utilities::split(std::begin(row)+mainBagEndPos+13, std::end(row)-5, ' ');
        std::vector<std::pair<int, std::string>> containedBags;
        if(split[0] != "no"){
            for(auto i=0u; i<split.size(); i+=4){
                containedBags.emplace_back(std::stoi(split[i]), split[i+1]+' '+split[i+2]);
            }
        }
        bagRules.emplace(std::move(mainBag), std::move(containedBags));
    };
    return bagRules;
};

auto processRules(const auto& bagRules){
    std::unordered_map<std::string, std::pair<bool, unsigned long>> bagInfo{bagRules.size()};
    auto keyRange = bagRules | std::views::keys;
    std::vector<std::reference_wrapper<const std::string>> bagsToProcess{keyRange.begin(), keyRange.end()};

    auto bagProcessed = [&bagRules, &bagInfo](const auto& bag){
        auto subBagInfoAvailable = [&bagInfo](const auto& containedBag){ return bagInfo.contains(containedBag); };
        const bool allSubBagInfosAvailable = std::ranges::all_of(bagRules.at(bag) | std::views::values, subBagInfoAvailable );

        if(allSubBagInfosAvailable){
            auto bagContainsShinyGold = [&bagInfo](const auto& containedBag){
                return containedBag == "shiny gold" || bagInfo[containedBag].first == true;
            };
            const bool containsShinyGold = std::ranges::any_of(bagRules.at(bag) | std::views::values, bagContainsShinyGold );

            const auto containedBags = Utilities::sum(bagRules.at(bag), 1ul, [&bagInfo](const auto& pair){
                return pair.first*bagInfo[pair.second].second;
            });
            bagInfo[bag] = std::make_pair(containsShinyGold, containedBags);
            return true;
        }
        return false;
    };
    while(!bagsToProcess.empty()){
        std::erase_if(bagsToProcess, bagProcessed);
    }
    const auto sumOfBagsContainingShinyGold  = Utilities::sum(bagInfo | std::views::elements<1> | std::views::elements<0>);
    const auto sumOfBagsContainedInShinyGold = bagInfo["shiny gold"].second-1;
    return std::make_pair(sumOfBagsContainingShinyGold, sumOfBagsContainedInShinyGold);
}

int main(){
    const auto bagRules = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto [sumOfBagsContainingShinyGoldBag, sumOfBagsContainedInShinyGold] = processRules(bagRules);
    std::cout << "There are a total of " << sumOfBagsContainingShinyGoldBag << " bags that contain the shiny gold bag.\n";

    //Task 2
    std::cout << "There are a total of " << sumOfBagsContainedInShinyGold << " bags contained inside the shiny gold bag.\n";

    VerifySolution::verifySolution(sumOfBagsContainingShinyGoldBag, sumOfBagsContainedInShinyGold);
}