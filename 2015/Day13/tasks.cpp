
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ranges>


auto fillHappinessMatrix = [](const auto& input){
    const auto N = (size_t)(1+std::sqrt(4*input.size()+1))/2;
    Matrix::Matrix<int> happinessMatrix(N, N);

    for(const auto i : std::views::iota(0u,N)){
        for(const auto j : std::views::iota(0u,i)){
            const auto split = Utilities::split(input[i*(N-1)+j]);
            happinessMatrix(i,j) = std::stoi(split[3]);
            if(split[2] == "lose") happinessMatrix(i,j)*=-1;
        }
        for(const auto j : std::views::iota(i+1, N)){
            const auto split = Utilities::split(input[i*(N-1)+j-1]);
            happinessMatrix(i,j) = std::stoi(split[3]);
            if(split[2] == "lose") happinessMatrix(i,j)*=-1;
        }
    }
    return happinessMatrix;
};

auto makeEveryoneHappy = [](const auto& happinessMatrix){
    const auto N = happinessMatrix.rows();
    std::vector<size_t> order(N-1);
    std::iota(std::begin(order), std::end(order), 0u);
    int maxHappiness = 0;
    do{
        auto happiness = happinessMatrix(order[N-2], N-1) + happinessMatrix(N-1, order[N-2]);
        auto lastPerson = N-1;
        for(const auto nextPerson : order){
            happiness+=happinessMatrix(lastPerson,nextPerson);
            happiness+=happinessMatrix(nextPerson,lastPerson);
            lastPerson=nextPerson;
        }
        if(happiness > maxHappiness) maxHappiness=happiness;
    }while(std::ranges::next_permutation(order).found);
    return maxHappiness;
};

auto addMyself = [](auto& happinessMatrix){
    const auto N = happinessMatrix.rows();
    happinessMatrix.resize(N+1, N+1);
};

int main(){
    auto happinessMatrix = fillHappinessMatrix(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto happiness = makeEveryoneHappy(happinessMatrix);
    std::cout << "Optimal change in happiness amounts to " << happiness << ".\n";

    //Task 2
    addMyself(happinessMatrix);
    const auto happinessWithMe = makeEveryoneHappy(happinessMatrix);
    std::cout << "Myself included the optimal change in happiness amounts to " << happinessWithMe << ".\n";
}