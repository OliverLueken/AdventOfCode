
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <deque>
#include <algorithm>
#include <numeric>
#include <climits>
#include <ranges>

using Position = Matrix::Position<size_t>;

auto parseInput = [](const auto& input){
    Matrix::Matrix<int> riskLevelMatrix{input.size(), input[0].size(),
        input | std::views::join | std::views::transform([](const auto& c) ->int {return c-'0';})
    };
    return riskLevelMatrix;
};

auto bfs = [](const auto& riskLevelMatrix){
    Matrix::Matrix<int> lowestRiskLevelMatrix{riskLevelMatrix.rows(), riskLevelMatrix.cols(), INT_MAX};
    lowestRiskLevelMatrix(0,0) = 0u;
    std::deque<Position> search{};
    search.emplace_back(0,0);

    while(!search.empty()){
        const auto [i,j] = search.front();
        search.pop_front();
        const auto currentRisk = lowestRiskLevelMatrix(i, j);
        const auto neighbors = getNeighbors(lowestRiskLevelMatrix, i, j);
        for(const auto& neighbor : neighbors){
            const auto neighborRiskLevel = riskLevelMatrix(neighbor.first, neighbor.second);
            if(currentRisk + neighborRiskLevel < lowestRiskLevelMatrix(neighbor.first, neighbor.second)){
                lowestRiskLevelMatrix(neighbor.first, neighbor.second) = currentRisk + neighborRiskLevel;
                search.emplace_back(neighbor.first, neighbor.second);
            }
        }
    }
    return lowestRiskLevelMatrix(lowestRiskLevelMatrix.rows()-1, lowestRiskLevelMatrix.cols()-1);
};

auto getLowestRiskOfCrossing = [](const auto& matrix){
    return bfs(matrix);
};

auto getCompleteRiskLevelMatrix = [](const auto& matrix){
    Matrix::Matrix<int> completeMatrix{5*matrix.rows(), 5*matrix.cols()};
    for(auto i=0u; i<completeMatrix.rows(); i++){
        auto ihigher = i/matrix.rows();
        for(auto j=0u; j<completeMatrix.cols(); j++){
            auto jhigher=j/matrix.cols();
            completeMatrix(i,j) = (int)(matrix(i%matrix.rows(), j%matrix.cols())+ihigher+jhigher-1)%9+1;
        }
    }
    return completeMatrix;
};

int main(){
    const auto riskLevelMatrix = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto lowestRiskOfCrossingCave = getLowestRiskOfCrossing(riskLevelMatrix);
    std::cout << "The lowest-risk-path entails a total risk of " << lowestRiskOfCrossingCave << ".\n";

    //Task 2
    const auto completeRiskLevelMatrix = getCompleteRiskLevelMatrix(riskLevelMatrix);
    const auto lowestRiskOfCrossingCompleteCave = getLowestRiskOfCrossing(completeRiskLevelMatrix);
    std::cout << "For the complete cave, the lowest risk is " << lowestRiskOfCrossingCompleteCave << ".\n";
}