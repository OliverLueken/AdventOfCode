
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <array>
#include <unordered_map>
#include <algorithm>

auto parseInput = [](const auto& input){
    std::array<int, 2> playerPosition;
    playerPosition[0] = input[0].back()-'0';
    playerPosition[1] = input[1].back()-'0';
    return playerPosition;
};

auto deterministicDie = []{
    static int value = 0;
    value = value%100+1;
    return value;
};

auto rollDeterministicDieThrice = []{
    return deterministicDie()+deterministicDie()+deterministicDie();
};

auto getResultOfTheFirstGame = [](auto playerPosition){
    std::array<int, 2> points{};
    auto rolls=0u;
    while(true){
        for(auto player=0u; player<playerPosition.size(); player++){
            playerPosition[player]=(playerPosition[player]+rollDeterministicDieThrice()-1)%10+1;
            points[player]+=playerPosition[player];
            rolls+=3;
            if(points[player]>=1000){
                return std::ranges::min(points)*rolls;
            }
        }
    }
    return 0u;
};


void dirac(const auto player, auto& playerPosition, auto& points, auto& wins, const auto universes, const auto rollToUniverses){
    for(const auto& [roll, uni] : rollToUniverses){
        const auto temp = playerPosition[player];
        playerPosition[player]=(playerPosition[player]+roll-1)%10+1;
        points[player]+=playerPosition[player];
        if(points[player]>=21){
            wins[player]+=universes*uni;
        }
        else{
            dirac(1-player, playerPosition, points, wins, universes*uni, rollToUniverses);
        }
        points[player]-=playerPosition[player];
        playerPosition[player]=temp;
    }
}

auto getMaxUniverseWins = [](auto playerPosition){
    std::array<int, 2> points{};
    std::array<unsigned long long, 2> wins{};
    const std::unordered_map<int, unsigned long long> rollToUniverses{
        {3,1},
        {4,3},
        {5,6},
        {6,7},
        {7,6},
        {8,3},
        {9,1}
    };
    dirac(0, playerPosition, points, wins, 1ull, rollToUniverses);
    return std::ranges::max(wins);
};

int main(){
    const auto playerPosition = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto result = getResultOfTheFirstGame(playerPosition);
    std::cout << "The resulting score of the first game is " << result << ".\n";

    // //Task 2
    const auto maxUniverseWins = getMaxUniverseWins(playerPosition);
    std::cout << "The player with the most wins won in " << maxUniverseWins << " universes.\n";

    VerifySolution::verifySolution(result, maxUniverseWins);
}