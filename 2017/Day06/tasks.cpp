
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

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

template<std::ranges::range Range>
struct circularView : public std::ranges::borrowed_iterator_t<Range>{

};

auto getResult = [](auto memory){
    auto hash = std::unordered_set<size_t>{};
    auto count = 0;
    while(hash.insert(getHash(memory)).second){
        auto maxPos = std::ranges::max_element(memory);
        auto val = *maxPos;
        *maxPos = 0;
        auto currentPos = maxPos+1;
        while(val>0){
            if(currentPos == memory.end()){
                currentPos = memory.begin();
            }
            ++*currentPos;
            --val;
            ++currentPos;
        }
        ++count;
    }
    return count;
};

auto getResult2 = [](const auto& memory){

    return 0;
};

int main(){
    const auto parsedInput = readFile::vectorOfInts("input.txt", '\t');

    //Task 1
    const auto result = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(parsedInput);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}