
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

Coroutines::Generator<std::bitset<16>> Gen(unsigned long val, const unsigned long multiplier, const auto divisibleBy){
    while(true){
        val = (val*multiplier)%2147483647;
        auto bin = std::bitset<16>{val};
        if(val%divisibleBy==0){
            co_yield bin;
        }
    }
}

class Judge{
    Coroutines::Generator<std::bitset<16>> gen1;
    Coroutines::Generator<std::bitset<16>> gen2;

public:
    Judge(auto init1, auto mult1, auto init2, auto mult2, int div1 = 1, int div2 = 1)
    : gen1{Gen(init1, mult1, div1)}, gen2{Gen(init2, mult2, div2)}{}

    auto operator()(unsigned long rounds) -> size_t {
        auto count = 0;
        while(rounds>0){
            count+=this->gen1() == this->gen2();
            --rounds;
        }
        return count;
    }
};


auto parseInput = [](const auto& input){
    auto init1 = std::stoi(Utilities::split(input[0]).back());
    auto init2 = std::stoi(Utilities::split(input[1]).back());
    return std::make_pair(init1, init2);
};

auto getResult = [](const auto init1, const auto init2){
    auto judge = Judge{init1, 16807, init2, 48271};
    return judge(40'000'000ul);
};

auto getResult2 = [](const auto init1, const auto init2){
    auto judge = Judge{init1, 16807, init2, 48271, 4, 8};
    return judge(5'000'000ul);
};

int main(){
    const auto [init1, init2] = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result = getResult(init1, init2);
    std::cout << "Task 1: " << result << ".\n";

    //Task 2
    const auto result2 = getResult2(init1, init2);
    std::cout << "Task 2: " << result2 << ".\n";

    VerifySolution::verifySolution(result, result2);
}