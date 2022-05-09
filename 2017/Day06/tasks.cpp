
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/views.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <ranges>
#include <sstream>


size_t getHash(const auto& memory){
    auto stream = std::stringstream{};
    for(const auto& val : memory){
        stream << val;
    }
    return std::hash<std::string>{}(stream.str());
}

auto getResult = [](auto memory){
    auto hash = std::unordered_map<size_t, int>{};
    auto count = 0;
    hash.insert_or_assign(getHash(memory), count);
    while(true){
        auto range = views::circle(memory);
        auto maxPos = std::ranges::max_element(range | std::views::take(memory.size()));
        auto val = *maxPos;
        *maxPos = 0;
        auto currentPos = ++maxPos;
        while(val>0){
            ++*currentPos;
            --val;
            ++currentPos;
        }
        ++count;
        
        const auto newhash = getHash(memory);
        if(hash.contains(newhash)){
            return std::make_pair(count, count-hash[newhash]);
        }
        hash[newhash]=count;
    }
};

int main(){
    const auto parsedInput = readFile::vectorOfInts("input.txt", '\t');

    //Task 1
    const auto [result, result2] = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";
    std::cout << "Task 2: " << result2 << ".\n";

    VerifySolution::verifySolution(result, result2);
}