
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


auto parseInput = [](const auto& input){
    std::vector<int> parsed{};
    auto split = Utilities::split(input, ',');
    std::ranges::transform(split, std::back_inserter(parsed), [](const auto& s){return std::stoi(s);});
    return parsed;
};

auto getResult = [](const auto& parsedInput){
    std::vector<int> numbers(256);
    std::iota(numbers.begin(), numbers.end(), 0);

    auto skipSize = 0;
    auto circle = views::circle(numbers);
    auto it = circle.begin();
    for(const auto& length : parsedInput){
        std::ranges::reverse(it, it+length);
        it+=length+skipSize;
        ++skipSize;
    }

    return numbers[0]*numbers[1];
};

auto getResult2 = [](const auto& parsedInput){

    return 0;
};

int main(){
    const auto parsedInput = parseInput(readFile::string());

    //Task 1
    const auto result = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(parsedInput);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}