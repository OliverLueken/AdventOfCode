
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

    auto operator()(unsigned long rounds) -> size_t {
        return Utilities::sum(std::views::iota(0ul, rounds), 0, [this](const auto){
            return this->gen1() == this->gen2();
        });
    }
};


auto parseInput = [](const auto& input){
    auto init1 = std::stoi(Utilities::split(input[0]).back());
    auto init2 = std::stoi(Utilities::split(input[1]).back());
    return Judge{init1, 16807, init2, 48271};
};

auto getResult = [](auto& parsedInput){
    return parsedInput(40'000'000ul);
};

auto getResult2 = [](const auto& parsedInput){

    return 0;
};

int main(){
    auto parsedInput = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(parsedInput);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}