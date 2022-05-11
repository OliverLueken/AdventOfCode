
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

struct Disc{
    int weight{};
    std::string name{};
    std::vector<std::string> discsAbove{};
};

auto parseInput = [](const auto& input){
    std::vector<Disc> discs{};
    for(const auto& row : input){
        auto split = Utilities::splitOnEach(row, " ,()->");
        auto name = std::move(split[0]);
        auto weight = std::stoi(split[1]);
        split.erase(split.begin(), split.begin()+2);
        discs.emplace_back(weight, std::move(name), std::move(split));
    }
    return discs;
};

auto getResult = [](const auto& discs){
    for(const auto& disc : discs){
        std::cout << disc.name << ' ' << disc.weight << ' ';
        for(const auto& discAbove : disc.discsAbove){
            std::cout << discAbove << ' ';
        }
        std::cout << '\n';
    }
    return 0;
};

auto getResult2 = [](const auto& discs){

    return 0;
};

int main(){
    const auto discs = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result = getResult(discs);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(discs);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}