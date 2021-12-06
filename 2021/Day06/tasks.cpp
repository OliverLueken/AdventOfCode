
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>


auto getPopulationAfter = [](const auto N, const auto& startingPopulation){
    std::unordered_map<int, unsigned long long> birthdayToFishCreatedMap{};
    for(int birthday=N; birthday>=-8; birthday--){
        if(!birthdayToFishCreatedMap.contains(birthday)){
            auto fishCreated = 1ull;
            for(int j=birthday+9; j<=N; j+=7){
                fishCreated+=birthdayToFishCreatedMap[j];
            }
            birthdayToFishCreatedMap[birthday]=fishCreated;
        }
    }

    const auto finalPopulation = Utilities::sum(startingPopulation, 0ull,
        [&birthdayToFishCreatedMap](const auto val){ return birthdayToFishCreatedMap[val-8]; }
    );
    return finalPopulation;
};

int main(){
    const auto startingPopulation = readFile::vectorOfInts("input.txt", ',');

    //Task 1
    const auto populationAfter80Days = getPopulationAfter(80, startingPopulation);
    std::cout << "There are " << populationAfter80Days << " lanternfish after 80 days\n";

    //Task 2
    const auto populationAfter256Days = getPopulationAfter(256, startingPopulation);
    std::cout << "There are " << populationAfter256Days << " lanternfish after 256 days\n";
}