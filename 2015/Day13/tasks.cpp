
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ranges>

auto fillHappinessMatrix = [](const auto& input){

    size_t N = (size_t)(1+std::sqrt(4*input.size()+1))/2;
    std::vector<std::vector<int>> happinessMatrix(N, std::vector<int>(N));

    for(size_t i : std::views::iota(0u,N)){
        for(size_t j=0; j<i; j++){
            auto split = Utilities::split(input[i*(N-1)+j]);
            happinessMatrix[i][j] = std::stoi(split[3]);
            if(split[2] == "lose") happinessMatrix[i][j]*=-1;
        }
        for(size_t j=i+1; j<N; j++){
            auto split = Utilities::split(input[i*(N-1)+j-1]);
            happinessMatrix[i][j] = std::stoi(split[3]);
            if(split[2] == "lose") happinessMatrix[i][j]*=-1;
        }
    }
    return happinessMatrix;
};

auto makeEveryoneHappy = [](const auto& happinessMatrix){
    size_t N =happinessMatrix.size();
    std::vector<size_t> order(N-1);
    std::iota(std::begin(order), std::end(order), 0u);
    int maxHappiness = 0;
    do{
        int happiness = happinessMatrix[order[N-2]][N-1] + happinessMatrix[N-1][order[N-2]];
        size_t lastPerson = N-1;
        for(size_t nextPerson : order){
            happiness+=happinessMatrix[lastPerson][nextPerson];
            happiness+=happinessMatrix[nextPerson][lastPerson];
            lastPerson=nextPerson;
        }
        if(happiness > maxHappiness) maxHappiness=happiness;
    }while(std::ranges::next_permutation(order).found);
    return maxHappiness;
};

auto addMyself = [](auto& happinessMatrix){
    size_t N = happinessMatrix.size();
    for(auto& v : happinessMatrix) v.push_back(0);
    happinessMatrix.push_back(std::vector<int>(N+1));
};

int main(){
    const auto input = readFile::vectorOfStrings("input.txt");

    auto happinessMatrix = fillHappinessMatrix(input);

    //Task 1
    auto happiness = makeEveryoneHappy(happinessMatrix);
    std::cout << "Optimal change in happiness amounts to " << happiness << ".\n";

    //Task 2
    addMyself(happinessMatrix);
    happiness = makeEveryoneHappy(happinessMatrix);
    std::cout << "Myself included the optimal change in happiness amounts to " << happiness << ".\n";
}