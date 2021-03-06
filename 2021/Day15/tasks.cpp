
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <deque>
#include <algorithm>
#include <numeric>
#include <climits>
#include <ranges>

using Position = Utilities::Position<size_t>;

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
            const auto neighborRiskLevel = riskLevelMatrix(neighbor);
            if(currentRisk + neighborRiskLevel < lowestRiskLevelMatrix(neighbor)){
                lowestRiskLevelMatrix(neighbor) = currentRisk + neighborRiskLevel;
                search.emplace_back(neighbor);
            }
        }
    }
    return lowestRiskLevelMatrix(lowestRiskLevelMatrix.rows()-1, lowestRiskLevelMatrix.cols()-1);
};

auto getLowestRiskOfCrossing = [](const auto& matrix){
    return bfs(matrix);
};

auto getCompleteRiskLevelMatrix = [](const auto& matrix){
    const auto n = matrix.rows();
    const auto m = matrix.cols();
    Matrix::Matrix<int> completeMatrix{5*n, 5*m};
    for(auto row=0u; row<completeMatrix.rows(); row++){
        const auto horizontalRiskOffset = row/n;
        for(auto col=0u; col<completeMatrix.cols(); col++){
            const auto verticalRiskOffset=col/m;
            completeMatrix(row,col) = (int)(matrix(row%n, col%m)+horizontalRiskOffset+verticalRiskOffset-1)%9+1;
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

    VerifySolution::verifySolution(lowestRiskOfCrossingCave, lowestRiskOfCrossingCompleteCave);
}