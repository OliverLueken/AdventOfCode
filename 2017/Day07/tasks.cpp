
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

auto getResult2 = [](const auto& discs){

    return 0;
};

int main(){
    const auto discs = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto baseDisc = getBaseDisc(discs);
    std::cout << "The program at the bottom is called " << baseDisc << ".\n";

    // //Task 2
    // const auto result2 = getResult2(discs);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(baseDisc, result2);
}