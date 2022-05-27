
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/coroutines.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

Coroutines::Generator<std::bitset<16>> Gen(unsigned long initializer, unsigned long multiplier){
    auto val = initializer;
    while(true){
        val = (val*multiplier)%2147483647;
        auto bin = std::bitset<16>{val};
        co_yield bin;
    }
}

class Judge{
    Coroutines::Generator<std::bitset<16>> gen1;
    Coroutines::Generator<std::bitset<16>> gen2;

public:
    Judge(auto init1, auto mult1, auto init2, auto mult2)
    : gen1{Gen(init1, mult1)}, gen2{Gen(init2, mult2)}{}

    bool compare(const std::string& val1, const std::string& val2) const {
        auto getLast16Bits = [](const auto& val){
            return val | std::views::reverse | std::views::take(16);
        };
        const auto lastBitsOfVal1 = getLast16Bits(val1);
        const auto lastBitsOfVal2 = getLast16Bits(val2);
        return std::ranges::mismatch(lastBitsOfVal1, lastBitsOfVal2).first == lastBitsOfVal1.end();
    }
};


auto parseInput = [](const auto& input){
    std::vector<> parsed;
    for(const auto& row : input){

    }
    return parsed;
};

auto getResult = [](const auto& parsedInput){

    return 0;
};

auto getResult2 = [](const auto& parsedInput){

    return 0;
};

int main(){
    const auto parsedInput = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(parsedInput);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}