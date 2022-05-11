
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

class Memory{

public:
    std::vector<int> mem{};
    constexpr Memory(std::vector<int>&& _mem) : mem{std::move(_mem)}{}

    constexpr auto operator==(const Memory& other) const {
        return mem == other.mem;
    }
};

template<>
struct std::hash<Memory>{
    std::size_t operator()(const Memory& memory) const noexcept{
        auto stream = std::stringstream{};
        for(const auto& val : memory.mem){
            stream << val;
        }
        return std::hash<std::string>{}(stream.str());
    }
};

auto getResult = [](auto memory){
    auto hash = std::unordered_map<Memory, int>{};
    auto count = 0;
    hash.insert_or_assign(memory, count);
    while(true){
        auto range = views::circle(memory.mem);
        auto maxPos = std::ranges::max_element(range | std::views::take(memory.mem.size()));
        auto val = *maxPos;
        *maxPos = 0;
        auto currentPos = ++maxPos;
        while(val>0){
            ++*currentPos;
            --val;
            ++currentPos;
        }
        ++count;

        // const auto newhash = getHash(memory);
        if(hash.contains(memory)){
            return std::make_pair(count, count-hash[memory]);
        }
        hash[memory]=count;
    }
};

int main(){
    const auto parsedInput = Memory{readFile::vectorOfInts("input.txt", '\t')};

    //Task 1
    const auto [result, result2] = getResult(parsedInput);
    std::cout << "Task 1: " << result << ".\n";
    std::cout << "Task 2: " << result2 << ".\n";

    VerifySolution::verifySolution(result, result2);
}