
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <stack>

auto makeStacks(){
    auto stacks = std::vector<std::vector<char>>{
        {'H', 'C', 'R'},
        {'B', 'J', 'H', 'L', 'S', 'F'},
        {'R', 'M', 'D', 'H', 'J', 'T', 'Q'},
        {'S', 'G', 'R', 'H', 'Z', 'B', 'J'},
        {'R', 'P', 'F', 'Z', 'T', 'D', 'C', 'B'},
        {'T', 'H', 'C', 'G'},
        {'S', 'N', 'V', 'Z', 'B', 'P', 'W', 'L'},
        {'R', 'J', 'Q', 'G', 'C'},
        {'L', 'D', 'T', 'R', 'H', 'P', 'F', 'S'},
    };
    return stacks;
}

struct Instruction{
    int n;
    int source;
    int destination;
};

auto parseInput = [](const auto& input){
    
    std::vector<Instruction> parsed;
    for(const auto& row : input){
        auto split = Utilities::split(row);
        parsed.emplace_back(Instruction{
            std::stoi(split[1]),
            std::stoi(split[3])-1,
            std::stoi(split[5])-1 
        });
        std::cout << parsed.back().n << ' ' 
                  << parsed.back().source << ' ' 
                  << parsed.back().destination << '\n';
    }
    return parsed;
};

auto getResult = [](auto stacks, const auto& instructions){
    
    for(const auto& instruction : instructions){
        for(auto i=0; i<instruction.n; ++i){
            std::cout << "moving " << instruction.source << " to " << instruction.destination << '\n';
            stacks[instruction.destination].push_back(stacks[instruction.source].back());
            stacks[instruction.source].pop_back();
        } 
    }
    
    for(auto& stack : stacks){
        while(not stack.empty()){
            std::cout << stack.back();
            stack.pop_back();
        }
        std::cout << '\n';
    }
    return 0;
};

auto getResult2 = [](auto stacks, const auto& instructions){

    
    for(const auto& instruction : instructions){
        std::cout << "moving " << instruction.source << " to " << instruction.destination << '\n';
        auto& dest = stacks[instruction.destination];
        auto& source = stacks[instruction.source];
        const auto n = instruction.n;
        dest.insert(dest.end(), source.end()-n, source.end());
        source.erase(source.end()-n, source.end());
    }
    
    for(auto& stack : stacks){
        while(not stack.empty()){
            std::cout << stack.back();
            stack.pop_back();
        }
        std::cout << '\n';
    }
    return 0;
};

int main(){
    auto stacks = makeStacks();
    const auto parsedInput = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result = getResult(stacks, parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    //Task 2
    const auto result2 = getResult2(stacks, parsedInput);
    std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}
