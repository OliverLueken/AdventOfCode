
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <set>
#include <unordered_map>

struct Disc{
    int weight{};
    std::vector<std::string> discsAbove{};
};

auto parseInput = [](const auto& input){
    std::unordered_map<std::string, Disc> discs{};
    for(const auto& row : input){
        auto split = Utilities::splitOnEach(row, " ,()->");
        auto name = std::move(split[0]);
        auto weight = std::stoi(split[1]);
        split.erase(split.begin(), split.begin()+2);
        discs.emplace(std::make_pair(
            std::move(name),
            Disc{weight, std::move(split)}
        ));
    }
    return discs;
};

auto getBaseDisc = [](const auto& discs){

    auto baseDiscs = std::set<std::string>{};
    std::ranges::copy(
        discs | std::views::filter([](const auto& disc){return !disc.second.discsAbove.empty();}) | std::views::keys,
        std::inserter(baseDiscs, std::begin(baseDiscs))
    );
    auto topDiscs  = std::set<std::string>{};
    std::ranges::copy(
        discs | std::views::values | std::views::transform(&Disc::discsAbove) | std::views::join,
        std::inserter(topDiscs, std::begin(topDiscs))
    );

    auto baseDisc = std::set<std::string>{};
    std::ranges::set_difference(baseDiscs, topDiscs, std::inserter(baseDisc, std::begin(baseDisc)));

    return *baseDisc.begin();
};

auto getOffWeight(const auto& discs, auto& weights, const auto& weightOf){
    if(weights.size() > 1){
        auto commonWeight = 0;
        auto offDisc = std::string{};
        for(const auto& [weight, discsOfThisWeight] : weights){
            if(discsOfThisWeight.size() == 1){
                offDisc = discsOfThisWeight.front();
            }
            else{
                commonWeight = weightOf.at(discsOfThisWeight.front());
            }
        }
        auto neededWeight = discs.at(offDisc).weight + (commonWeight - weightOf.at(offDisc));
        return std::make_pair(true, neededWeight);
    }
    return std::make_pair(false, 0);
}

auto getWeight(const auto& discs, const auto& disc) -> std::pair<bool, int> {
    static auto weightOf = std::unordered_map<std::string, int>{};

    if(!weightOf.contains(disc)){
        weightOf[disc] = discs.at(disc).weight;
        auto weightsOfSons = std::unordered_map<int, std::vector<std::string>>{};
        for(const auto& son : discs.at(disc).discsAbove){
            auto [weightOff, weight] = getWeight(discs, son);
            if(weightOff){
                return std::make_pair(true, weight);
            }
            weightsOfSons[weight].push_back(son);
            weightOf[disc]+=weight;
        }

        auto [weightOff, weight] = getOffWeight(discs, weightsOfSons, weightOf);
        if(weightOff){
            return std::make_pair(true, weight);
        }
    }
    return std::make_pair(false, weightOf[disc]);
}

auto getCorrectWeight(const auto& discs, const auto& base){

    return getWeight(discs, base).second;
};

int main(){
    const auto discs = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto baseDisc = getBaseDisc(discs);
    std::cout << "The program at the bottom is called " << baseDisc << ".\n";

    //Task 2
    const auto correctedWeight = getCorrectWeight(discs, baseDisc);
    std::cout << "Task 2: " << correctedWeight << ".\n";

    VerifySolution::verifySolution(baseDisc, correctedWeight);
}