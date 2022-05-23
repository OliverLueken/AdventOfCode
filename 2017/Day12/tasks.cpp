
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <ranges>

class Network{
public:
    std::unordered_map<int, std::vector<int>> connections{};
};

auto parseInput = [](const auto& input){
    auto network = Network{};
    for(const auto& row : input){
        const auto split = Utilities::splitOnEach(row, ", ");
        const auto id = std::stoi(split[0]);
        std::ranges::transform(split.begin()+2, split.end(), std::back_inserter(network.connections[id]), [](const auto& neighbor){
            return std::stoi(neighbor);
        });
    }
    return network;
};

auto getResult = [](const auto& network){
    return 0;
};

auto getResult2 = [](const auto& network){

    return 0;
};

int main(){
    const auto network = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result = getResult(network);
    std::cout << "Task 1: " << result << ".\n";

    // //Task 2
    // const auto result2 = getResult2(network);
    // std::cout << "Task 2: " << result2 << ".\n";

    // VerifySolution::verifySolution(result, result2);
}