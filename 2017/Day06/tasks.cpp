
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

    std::vector<int> mem{};

    auto max_element() {
        auto range = views::circle(mem);
        return std::ranges::max_element(range | std::views::take(mem.size()));
    }

public:
    Memory(std::vector<int>&& _mem) : mem{std::move(_mem)}{}

    auto operator==(const Memory& other) const {
        return mem == other.mem;
    }

    auto redistributeBlocks(){
        auto currentPos = max_element();
        auto blocks = *currentPos;
        *currentPos = 0;
        while(blocks>0){
            ++currentPos;
            ++*currentPos;
            --blocks;
        }
    }

    auto begin() const {
        return mem.begin();
    }
    auto end() const {
        return mem.end();
    }
};

template<>
struct std::hash<Memory>{
    std::size_t operator()(const Memory& memory) const noexcept{
        auto stream = std::stringstream{};
        for(const auto& val : memory){
            stream << val;
        }
        return std::hash<std::string>{}(stream.str());
    }
};

auto getMemoryLoopValues = [](auto& memory){
    auto memoryStateLastSeenAtCycle = std::unordered_map<Memory, int>{};
    auto currentCycle = 0;
    memoryStateLastSeenAtCycle.insert_or_assign(memory, currentCycle);
    while(true){
        memory.redistributeBlocks();
        ++currentCycle;

        if(memoryStateLastSeenAtCycle.contains(memory)){
            break;
        }
        memoryStateLastSeenAtCycle[memory]=currentCycle;
    }
    return std::make_pair(currentCycle, currentCycle-memoryStateLastSeenAtCycle[memory]);
};

int main(){
    auto memory = Memory{readFile::vectorOfInts("input.txt", '\t')};

    //Task 1 & 2
    const auto [redestributionCycles, loopPeriod] = getMemoryLoopValues(memory);
    std::cout << "It takes " << redestributionCycles << " cycles to reach a memory state a second time.\n";
    std::cout << "The memory state has a period of " << loopPeriod << " cycles.\n";

    VerifySolution::verifySolution(redestributionCycles, loopPeriod);
}