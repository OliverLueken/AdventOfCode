
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

auto getNext = [](const auto& lastSeenAtRound, const auto last, const auto round){
    if(lastSeenAtRound.contains(last)) {
        return round - lastSeenAtRound.at(last);
    }
    return 0;
};

auto getLastNumberSpoken = [](const auto& n, int num) {
    std::unordered_map<int, int> lastSeenAtRound;
    for(auto round=0u; round<n.size()-1; ++round) lastSeenAtRound[n[round]] = round+1;

    auto last = n.back();
    for(auto round=static_cast<int>(n.size()); round<num; ++round) {
        const auto next = getNext(lastSeenAtRound, last, round);
        lastSeenAtRound[last] = round;
        last = next;
    }
    return last;
};

int main() {
    const auto input = std::vector<int>{1, 20, 8, 12, 0, 14};

    // Task 1
    const auto spokenNumber2020 = getLastNumberSpoken(input, 2020);
    std::cout << "The 2020th number spoken is " << spokenNumber2020 << ".\n";

    // Task 2
    const auto spokenNumber30000000 = getLastNumberSpoken(input, 30000000);
    std::cout << "The 30.000.000th number spoken is " << spokenNumber30000000 << ".\n";

    VerifySolution::verifySolution(spokenNumber2020, spokenNumber30000000);
}
