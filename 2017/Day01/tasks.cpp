
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <numeric>
#include <climits>
#include <memory>
#include <ranges>
#include <cmath>
#include <deque>

using Position = Utilities::Position<int>;

auto parseInput = [](const auto& input){
    std::vector<int> parsed;
    std::ranges::transform(input, std::back_inserter(parsed), [](const auto c){return c-'0';});
    parsed.push_back(parsed.front());
    return parsed;
};

auto getResult = [](const auto& digits){
    auto sum = 0;
    for(auto it = digits.begin(); it!=digits.end()-1; ++it){
        if(*it == *(it+1)){
            sum+=*it;
        }
    }
    return sum;
};

auto getResult2 = [](const auto& digits){
    auto sum = 0;
    const auto offset = digits.size()/2;
    for(auto it = digits.begin(); it!=digits.end()-offset; ++it){
        if(*it == *(it+offset)){
            sum+=2* *it;
        }
    }
    return sum;
};

int main(){
    const auto digits = parseInput(readFile::string());

    //Task 1
    const auto nextNeighborSum = getResult(digits);
    std::cout << "The sum of digits matching their next neighbor is " << nextNeighborSum << ".\n";

    //Task 2
    const auto halfwayRoundNeighbor = getResult2(digits);
    std::cout << "The sum of digits matching the digit halfway around is " << halfwayRoundNeighbor << ".\n";

    VerifySolution::verifySolution(nextNeighborSum, halfwayRoundNeighbor);
}