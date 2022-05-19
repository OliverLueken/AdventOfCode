
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

auto oneRoundKnotHash(auto& circle, const auto lengths){
    static auto it = circle.begin();
    static auto skipSize = 0;
    for(const auto& length : lengths){
        std::ranges::reverse(it, it+length);
        it+=length+skipSize;
        ++skipSize;
    }
}

auto getResult = [](auto& numbers, const auto& input){
    const auto lengths = interpretInputAsListOfIntegers(input);

    auto circle = views::circle(numbers);
    oneRoundKnotHash(circle, lengths);

    return numbers[0]*numbers[1];
};

auto getResult2 = [](const auto& parsedInput){

    return 0;
};

int main(){
    const auto input = readFile::string();

    auto numbers = std::vector<int>(256);
    std::iota(numbers.begin(), numbers.end(), 0);

    //Task 1
    const auto result = getResult(numbers, input);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(input);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}