
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/views.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <sstream>
#include <iomanip>


auto interpretInputAsListOfIntegers = [](const auto& input){
    auto vecOfInts = std::vector<int>{};
    auto split = Utilities::split(input, ',');
    std::ranges::transform(split, std::back_inserter(vecOfInts), [](const auto& s){return std::stoi(s);});
    return vecOfInts;
};

auto interpretInputAsListOfBytes(const auto& input){
    auto vecOfInts = std::vector<int>{};
    std::ranges::transform(input, std::back_inserter(vecOfInts), [](const auto& s){return static_cast<int>(s);});
    vecOfInts.push_back(17);
    vecOfInts.push_back(31);
    vecOfInts.push_back(73);
    vecOfInts.push_back(47);
    vecOfInts.push_back(23);
    return vecOfInts;
}

struct KnotHash{
    Views::iterator<std::vector<int>> iterator{};
    int skipSize{0};
    std::vector<int> lengths{};
    std::vector<int> numbers{};

    KnotHash(std::vector<int>&& _lengths)
        : skipSize{0}, lengths{std::move(_lengths)}, numbers(std::vector<int>(256)){
        std::iota(numbers.begin(), numbers.end(), 0);
        auto circle = Views::circle(numbers);
        iterator = circle.begin();
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

auto getProductOfFirstTwoValuesAfterOneRoundOfHashKnotting = [](const auto& input){
    auto hashMaker = KnotHash{interpretInputAsListOfIntegers(input)};
    hashMaker.oneRoundKnotHash();

    const auto firstValue  = *hashMaker.begin();
    const auto secondValue = *(hashMaker.begin()+1);
    return firstValue*secondValue;
};

auto getKnotHash = [](const auto& input){
    auto hashMaker = KnotHash{interpretInputAsListOfBytes(input)};
    return hashMaker.makeHash();
};

int main(){
    const auto input = readFile::string();

    //Task 1
    const auto product = getProductOfFirstTwoValuesAfterOneRoundOfHashKnotting(input);
    std::cout << "The product of the first two numbers in the list after knotting is " << product << ".\n";

    //Task 2
    const auto knotHash = getKnotHash(input);
    std::cout << "The Knot Hash is " << knotHash << ".\n";

    VerifySolution::verifySolution(product, knotHash);
}