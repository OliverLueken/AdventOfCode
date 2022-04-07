
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
    return parsed;
};

auto sumIfEqual(const auto& range, const auto offset){
    auto sum = 0;
    for(auto i=0ul; i<range.size(); ++i){
        if(range[i] == range[(i+offset)%range.size()] ){
            sum+=range[i];
        }
    }
    return sum;
}

int main(){
    const auto digits = parseInput(readFile::string());

    //Task 1
    const auto nextNeighborSum = sumIfEqual(digits, 1);
    std::cout << "The sum of digits matching their next neighbor is " << nextNeighborSum << ".\n";

    //Task 2
    const auto halfwayRoundNeighbor = sumIfEqual(digits, digits.size()/2);
    std::cout << "The sum of digits matching the digit halfway around is " << halfwayRoundNeighbor << ".\n";

    VerifySolution::verifySolution(nextNeighborSum, halfwayRoundNeighbor);
}