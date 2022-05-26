
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <ranges>

using Position = Utilities::Position<int>;

struct KnotHash{
    views::iterator<std::vector<int>> iterator{};
    int skipSize{0};
    std::vector<int> lengths{};
    std::vector<int> numbers{};

    KnotHash(std::vector<int>&& input)
        : skipSize{0}, lengths{interpretInputAsListOfBytes(std::move(input))}, numbers(std::vector<int>(256)){
        std::iota(numbers.begin(), numbers.end(), 0);
        auto circle = views::circle(numbers);
        iterator = circle.begin();
    }


    auto interpretInputAsListOfBytes(const auto& input) const {
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



auto getResult = [](const auto& input){

    return 0;
};

auto getResult2 = [](const auto& input){

    return 0;
};

int main(){
    const auto input = readFile::string();

    //Task 1
    const auto result = getResult(input);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(input);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}