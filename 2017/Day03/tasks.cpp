
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/coroutines.hpp"

#include <iostream>
#include <ranges>
#include <algorithm>
#include <span>

auto getLayer(const auto input){
    auto layer = 0;
    while(4*layer*layer+4*layer+1 < input){
        ++layer;
    }
    return layer;
}

auto getDistanceToCenter = [](const auto input){
    const auto layer = getLayer(input);
    const auto smallestNumberInLayer = 4*layer*(layer-1)+2;
    auto x = (input + layer-smallestNumberInLayer+1)%(2*layer);
    x = std::abs(x-layer);
    return 2*layer-x;
};

Coroutines::Generator<int> nextCorner(){
    int nextCorner = 1;
    int difference = 1;
    while(true){
        co_yield nextCorner;
        nextCorner+=difference;
        co_yield nextCorner;
        nextCorner+=difference;
        ++difference;
    }
}

Coroutines::Generator<std::vector<int>> nextNeighbors(){
    auto getNextCorner = nextCorner();
    static auto corners = std::vector<int>{getNextCorner(),getNextCorner(),getNextCorner(),getNextCorner()};
    auto currentPosition = 5;


    while(currentPosition < 100){
        corners.push_back(getNextCorner());
        const auto cornerCount = static_cast<int>(corners.size());

        const auto adjacentCorner = std::max(0, cornerCount-6);
        const auto nextAdjacentCorner = std::max(adjacentCorner, std::min(adjacentCorner+1, cornerCount-5) );
        auto windowBegin = -2;
        auto windowEnd   = 0;
        const auto max = corners[nextAdjacentCorner]-corners[adjacentCorner];
        auto nextNeighbors = std::vector<int>{};
        while(currentPosition < corners.back()){
            auto span = std::views::iota(
                corners[adjacentCorner]+std::max(windowBegin, 0),
                corners[adjacentCorner]+std::min(windowEnd, max)+1
            );
            std::ranges::copy(span, std::back_inserter(nextNeighbors));
            if(currentPosition-1 == corners[corners.size()-2]){
                nextNeighbors.push_back(currentPosition-2);
            }
            nextNeighbors.push_back(currentPosition-1);
            ++windowBegin;
            ++windowEnd;
            ++currentPosition;
            co_yield nextNeighbors;
            nextNeighbors.clear();
        }
    }
}

auto getNextHigherValue = [](const auto& input){
    auto valueAtPosition = std::unordered_map<int, int>{{1,1},{2,1},{3,2},{4,4}};
    auto getNeighbors = nextNeighbors();
    auto currentPosition = 4;
    while(valueAtPosition[currentPosition] < input){
        ++currentPosition;
        const auto neighbors = getNeighbors();
        valueAtPosition[currentPosition] = Utilities::sum(neighbors, 0, [&valueAtPosition](const auto& i){return valueAtPosition[i];});
    }
    return valueAtPosition[currentPosition];
};

int main(){
    const auto input = 325489;

    //Task 1
    const auto distanceToCenter = getDistanceToCenter(input);
    std::cout << "The input square is " << distanceToCenter << " units away from the center.\n";

    //Task 2
    const auto nextHigherValue = getNextHigherValue(input);
    std::cout << "The value in the sequence after the input is " << nextHigherValue << ".\n";

    VerifySolution::verifySolution(distanceToCenter, nextHigherValue);
}
