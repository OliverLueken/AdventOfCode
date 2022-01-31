
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <bitset>
#include <string>

struct MaskAndMemoryInput{
    std::string mask{};
    std::vector<std::pair<uint64_t, std::bitset<36>>> memory{};
};

auto dock_version1(const auto& maskAndMemoryInput) {

    auto makeMask = [](auto mask, const auto val){
        std::ranges::replace(mask, 'X', val);
        return std::bitset<36>{mask};
    };

    std::unordered_map<uint64_t, std::bitset<36>> addressToValue{};

    for(const auto& MaskAndMemoryInput : maskAndMemoryInput){
        const auto mask0 = makeMask(MaskAndMemoryInput.mask, '0');
        const auto mask1 = makeMask(MaskAndMemoryInput.mask, '1');

        for(const auto& [address, val] : MaskAndMemoryInput.memory){
            addressToValue[address] = (val | mask0 ) & mask1;
        }
    }

    return Utilities::sum(addressToValue | std::views::values, 0ul, [](const auto& val){return val.to_ulong();});
}

auto addFlipped(auto& memoryAddresses, const auto pos){
    memoryAddresses.reserve(memoryAddresses.size()*2);
    const auto endIt = std::end(memoryAddresses);
    std::ranges::transform(std::begin(memoryAddresses), endIt, std::back_inserter(memoryAddresses), [pos](auto address){
        address.flip(pos);
        return address;
    });
}

auto applyMask(auto& memoryAddresses, const auto pos){
    std::ranges::for_each(memoryAddresses, [pos](auto& address){
        address.set(pos, true);
    });
}

auto buildMemoryAddresses(const auto& mask, const auto address){
    std::vector<std::bitset<36>> memoryAddresses{address};

    for(auto pos = 0; const auto& c : mask | std::views::reverse){
        switch(c){
            break; case 'X': addFlipped(memoryAddresses, pos);
            break; case '1': applyMask (memoryAddresses, pos);
        }
        ++pos;
    }
    return memoryAddresses;
}

auto addValueToAddresses(const auto& memoryAddresses, const auto& data, auto& addressToValue){
    const auto val = data.to_ulong();
    for (auto& address : memoryAddresses) {
        addressToValue[address] = val;
    }
}

auto dock_version2(const auto& maskAndMemoryInput) {
    std::unordered_map<std::bitset<36>, uint64_t> addressToValue;

    for(const auto& MaskAndMemoryInput : maskAndMemoryInput){
        const auto& mask = MaskAndMemoryInput.mask;
        for(const auto& [startingAddress, value] : MaskAndMemoryInput.memory){
            const auto memoryAddresses = buildMemoryAddresses(mask, startingAddress);
            addValueToAddresses(memoryAddresses, value, addressToValue);
        }
    }

    return Utilities::sum(addressToValue | std::views::values, 0ul);
}

auto parseInput(const auto& input){
    std::vector<MaskAndMemoryInput> maskAndMemoryInput{};
    for(const auto& row : input){
        auto split = Utilities::splitOnEach(row, "[= ");
        if(split.size()==2){
            maskAndMemoryInput.emplace_back( std::move(split[1]) );
        }
        else{
            maskAndMemoryInput.back().memory.emplace_back(std::stoul(split[1]), std::stoul(split[2]));
        }

    }
    return maskAndMemoryInput;
}

int main(){
    const auto maskAndMemoryInput = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto dock_version1_sum = dock_version1(maskAndMemoryInput);
    std::cout << "The sum of all values in the memory is " << dock_version1_sum <<  ".\n";

    //Task 2
    const auto dock_version2_sum = dock_version2(maskAndMemoryInput);
    std::cout << "With version 2, the sum of all values in the memory is " << dock_version2_sum <<  ".\n";

    VerifySolution::verifySolution(dock_version1_sum, dock_version2_sum);
}