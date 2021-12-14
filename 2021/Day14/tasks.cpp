
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <ranges>


auto parseInput = [](const auto& input){
    const auto polymer = input[0];
    std::unordered_map<std::string, char> rules;
    for(const auto& row : input | std::views::drop(1)){
        const auto split = Utilities::split(row);
        rules[split[0]] = split[2][0];
    }
    return std::make_pair(polymer, rules);
};

void countElements(const auto leftElement, const auto rightElement, const auto iterations,
                   const auto& rules, auto& elementCount, auto& elementCountCache){

    if(iterations == 0) return;
    const auto pair = std::string(1, leftElement) + rightElement;
    const auto key  = pair + std::to_string(iterations);

    if(!elementCountCache.contains(key)){
        std::unordered_map<char, long> newElementCountCache{};
        if(rules.contains(pair))
        {
            const auto middleElement = rules.at(pair);
            newElementCountCache[middleElement]++;
            countElements(leftElement  , middleElement, iterations-1, rules, newElementCountCache, elementCountCache);
            countElements(middleElement, rightElement , iterations-1, rules, newElementCountCache, elementCountCache);
        }
        elementCountCache.emplace(key, std::move(newElementCountCache));
    }
    for(const auto& [element, cacheCount] : elementCountCache[key]){
        elementCount[element]+=cacheCount;
    }
}

auto getPolymerElementCount = [](const auto& polymer, const auto& rules, const auto iterations){
    std::unordered_map<char, long> elementCount{};
    std::unordered_map<std::string, std::unordered_map<char, long>> elementCountCache{};

    for(auto leftElement = polymer[0];
        auto rightElement : polymer | std::views::drop(1)){
        countElements(leftElement, rightElement, iterations, rules, elementCount, elementCountCache);
        leftElement = rightElement;
    }

    for(const auto element : polymer){
        elementCount[element]++;
    }
    return elementCount;
};

auto getMostCommonLeastCommonElementDifference = [](const auto& polymer, const auto& rules, const auto iterations){
    const auto elementCount = getPolymerElementCount(polymer, rules, iterations);
    const auto [min, max] = std::ranges::minmax(elementCount | std::views::values);
    return max-min;
};

int main(){
    const auto [polymer, rules] = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto differenceAfter10Iteratins = getMostCommonLeastCommonElementDifference(polymer, rules, 10);
    std::cout << "After 10 iterations, the difference between the most common and least common elements is " << differenceAfter10Iteratins << ".\n";

    //Task 2
    const auto differenceAfter40Iterations = getMostCommonLeastCommonElementDifference(polymer, rules, 40);
    std::cout << "After 10 iterations, the difference between the most common and least common elements is " << differenceAfter40Iterations << ".\n";
}