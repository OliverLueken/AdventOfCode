
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
    Memory(std::vector<int>&& _mem) : mem{std::move(_mem)}{}

    auto operator==(const Memory& other) const {
        return mem == other.mem;
    }

    auto max_element() {
        auto range = views::circle(mem);
        return std::ranges::max_element(range | std::views::take(mem.size()));
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

auto getMemoryLoopValues = [](auto& memory){
    auto hash = std::unordered_map<Memory, int>{};
    auto count = 0;
    hash.insert_or_assign(memory, count);
    while(true){
        auto maxPos = memory.max_element();
        auto val = *maxPos;
        *maxPos = 0;
        auto currentPos = ++maxPos;
        while(val>0){
            ++*currentPos;
            --val;
            ++currentPos;
        }
        ++count;

        if(hash.contains(memory)){
            return std::make_pair(count, count-hash[memory]);
        }
        hash[memory]=count;
    }
};

int main(){
    auto memory = Memory{readFile::vectorOfInts("input.txt", '\t')};

    //Task 1
    const auto [redestributionCycles, loopPeriod] = getMemoryLoopValues(memory);
    std::cout << "It takes " << redestributionCycles << " cycles to reach a memory state a second time.\n";
    std::cout << "The memory state has a period of " << loopPeriod << " cycles.\n";

    VerifySolution::verifySolution(redestributionCycles, loopPeriod);
}