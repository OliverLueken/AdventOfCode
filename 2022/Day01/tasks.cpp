
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>


auto parseInput = [](const auto& input){
    std::vector<std::vector<int>> parsed;
    std::vector<int> currentElf;
    for(const auto& row : input){
	    if(row == ""){
	        parsed.emplace_back(std::move(currentElf));
	        currentElf.clear();
	    }
	    else{
	        currentElf.emplace_back(std::stoi(row));
        }
    }
    parsed.emplace_back(std::move(currentElf));
    return parsed;
};

auto getResult = [](const auto& sumOfCalories){
    return std::ranges::max(sumOfCalories);
};

auto getResult2 = [](auto& sumOfCalories){
    std::ranges::partial_sort(sumOfCalories, sumOfCalories.begin()+3, std::greater{});
    return sumOfCalories[0]+sumOfCalories[1]+sumOfCalories[2];
};

int main(){
    const auto parsedInput = parseInput(readFile::vectorOfStrings("input.txt", '\n', true));

    auto sumOfCalories = std::vector<int>{};
    std::ranges::transform(parsedInput, std::back_inserter(sumOfCalories), [](const auto& vec){
        return sum = Utilities::sum(vec);
    });
    
    //Task 1
    const auto result = getResult(sumOfCalories);
    std::cout << "Task 1: " << result << ".\n";

    //Task 2
    const auto result2 = getResult2(sumOfCalories);
    std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}
