
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/views.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>


auto interpretInputAsListOfIntegers = [](const auto& input){
    auto vecOfInts = std::vector<int>{};
    auto split = Utilities::split(input, ',');
    std::ranges::transform(split, std::back_inserter(vecOfInts), [](const auto& s){return std::stoi(s);});
    return vecOfInts;
};

auto interpretInputAsListOfBytes(const auto& input){
    auto vecOfInts = std::vector<int>{};
    std::ranges::transform(input, std::back_inserter(vecOfInts), [](const auto& s){return static_cast<int>(s);});
    vecOfInts.push_back(17);
    vecOfInts.push_back(31);
    vecOfInts.push_back(73);
    vecOfInts.push_back(47);
    vecOfInts.push_back(23);
    return vecOfInts;
}

struct KnotHash{
    views::iterator<std::vector<int>> it{};
    int skipSize{0};
    std::vector<int> lengths{};
    std::vector<int> numbers{};

    KnotHash(std::vector<int> _lengths)
        : skipSize{0}, lengths{std::move(_lengths)}, numbers(std::vector<int>(256)){
        std::iota(numbers.begin(), numbers.end(), 0);
        auto circle = views::circle(numbers);
        it = circle.begin();
    }

    auto oneRoundKnotHash(){
        for(const auto& length : lengths){
            std::ranges::reverse(it, it+length);
            it+=length+skipSize;
            ++skipSize;
        }
    }

    auto makeHash(){
        return 0;
    }

    auto begin(){
        return numbers.begin();
    }

    auto end(){
        return numbers.end();
    }
};

auto getResult = [](const auto& input){
    auto lengths = interpretInputAsListOfIntegers(input);
    auto hashMaker = KnotHash{lengths};
    hashMaker.oneRoundKnotHash();

    return std::accumulate(hashMaker.begin(), hashMaker.begin()+2, 1, std::multiplies<>{});
};

auto getResult2 = [](const auto& parsedInput){

    return 0;
};

int main(){
    const auto input = readFile::string();

    //Task 1
    const auto result = getResult(input);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(input);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}