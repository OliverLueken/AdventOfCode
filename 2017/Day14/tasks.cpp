
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"
#include "../../lib/views.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>
#include <bitset>
#include <deque>

struct KnotHash{
    views::iterator<std::vector<int>> iterator{};
    int skipSize{0};
    std::vector<int> lengths{};
    std::vector<int> numbers{};

    KnotHash(std::string&& input)
        : skipSize{0}, lengths{interpretInputAsListOfBytes(std::move(input))}, numbers(std::vector<int>(256)){
        std::iota(numbers.begin(), numbers.end(), 0);
        auto circle = views::circle(numbers);
        iterator = circle.begin();
    }


    std::vector<int> interpretInputAsListOfBytes(std::string&& input) const {
        auto vecOfInts = std::vector<int>{};
        std::ranges::transform(input, std::back_inserter(vecOfInts), [](const auto& s){return static_cast<int>(s);});
        vecOfInts.push_back(17);
        vecOfInts.push_back(31);
        vecOfInts.push_back(73);
        vecOfInts.push_back(47);
        vecOfInts.push_back(23);
        return vecOfInts;
    }

    auto oneRoundKnotHash(){
        for(const auto& length : lengths){
            std::ranges::reverse(iterator, iterator+length);
            iterator+=length+skipSize;
            ++skipSize;
        }
    }

    auto getDenseHash() const {
        auto denseHash = std::vector<int>{};
        for(auto _it = std::begin(numbers); _it!=std::end(numbers); _it+=16){
            denseHash.push_back(
                std::accumulate(_it+1, _it+16, *_it, [](const auto& lhs, const auto& rhs){return lhs^rhs;})
            );
        }
        return denseHash;
    }

    auto convertToHexadecimalString(const auto denseHash) const {
        auto stream = std::stringstream{};
        for(const auto& val : denseHash){
            stream << std::setfill('0') << std::setw(2) << std::hex << val;
        }
        return stream.str();
    }

    auto makeHash(){
        for(auto round=0; round<64; ++round){
            oneRoundKnotHash();
        }
        return convertToHexadecimalString(getDenseHash());
    }

    auto begin(){
        return numbers.begin();
    }

    auto end(){
        return numbers.end();
    }
};

auto hexToBin(const char c){
    std::stringstream ss{};
    ss << std::hex <<c;
    unsigned n;
    ss >> n;
    std::bitset<4> binary(n);
    return binary.to_string();
}

auto makeDisc(const auto& input){
    auto getMemoryRow = [](auto&& _input){
        auto hashMaker = KnotHash{std::move(_input)};
        auto hash = hashMaker.makeHash();

        auto stream = std::stringstream{};
        for(const auto& val : hash){
            stream << hexToBin(val);
        }
        return stream.str();
    };

    auto disc = std::vector<std::string>{};
    for(auto row : std::views::iota(0,128)){
        disc.emplace_back(getMemoryRow(input+'-'+std::to_string(row)));
    }
    return disc;
}

auto getResult = [](const auto& disc){
    return Utilities::sum(disc | std::views::join | std::views::transform([](const auto& c){
        return c-'0';
    }));
};

struct Memory{
    bool used{false};
    bool isGrouped{false};
};

auto getResult2 = [](const auto& disc){
    auto discGroups = Matrix::Matrix<Memory>{
        128, 128, disc | std::views::join | std::views::transform(
            [](const auto& c){
                return Memory{c=='1', false};
            }
        )
    };
    auto groupCount = 0;

    auto groupMemory = [&groupCount](auto& _discGroups, auto it){
        auto toVisit = std::deque<decltype(it)>{it};
        while(!toVisit.empty()){
            auto currentIt = toVisit.front();
            toVisit.pop_front();
            currentIt->isGrouped = true;

            std::ranges::copy_if(
                getNeighbors(_discGroups, currentIt),
                std::inserter(toVisit, std::begin(toVisit)),
                [](const auto _it){
                    return _it->used && !_it->isGrouped;
                }
            );
        }
    };

    for(auto it=discGroups.begin(); it!=discGroups.end(); ++it){
        if(it->used && !it->isGrouped){
            ++groupCount;
            groupMemory(discGroups, it);
        }
    }

    return groupCount;
};

int main(){
    const auto disc = makeDisc(readFile::string());

    //Task 1
    const auto result = getResult(disc);
    std::cout << "Task 1: " << result << ".\n";

    //Task 2
    const auto result2 = getResult2(disc);
    std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}