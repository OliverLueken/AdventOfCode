
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <climits>

#include "battleground.h"
#include "magician.h"
#include "enemy.h"
#include "spell.h"
#include "effect.h"


auto getLeastManaConsumption = [](auto enemy, bool hardDifficulty = false){

    Battleground battleground{std::move(enemy), hardDifficulty};
    auto minMana = UINT_MAX;
    unsigned int lastRound;
    do{
        lastRound = battleground.battle(minMana);
        battleground.reset();
    }while(battleground.magician.updateBattlePlan(lastRound));
    return minMana;
};

auto parseEnemyInput = [](const auto& input){
    const auto hp     = std::stoi(Utilities::split(input[0])[2]);
    const auto damage = std::stoi(Utilities::split(input[1])[1]);
    return Enemy{hp, damage};
};

int main(){
    const Enemy enemy = parseEnemyInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto minManaConsumption1 = getLeastManaConsumption(enemy);
    std::cout << "The fight can be won with at least " << minManaConsumption1 << " mana consumed.\n";

    //Task 2
    const auto minManaConsumption2 = getLeastManaConsumption(enemy, true);
    std::cout << "On hard difficulty, the fight can be won with at least " << minManaConsumption2 << " mana consumed.\n";

    VerifySolution::verifySolution(minManaConsumption1, minManaConsumption2);
}