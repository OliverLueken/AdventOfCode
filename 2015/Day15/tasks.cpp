
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

auto getIngredients = [](const auto& input){
    std::vector<std::vector<int>> ingredients;
    for(const auto& s : input){
        std::vector<int> ingredient;
        const auto split = Utilities::split(s);
        ingredient.push_back(std::stoi(split[2]));  //capacity
        ingredient.push_back(std::stoi(split[4]));  //durability
        ingredient.push_back(std::stoi(split[6]));  //flavor
        ingredient.push_back(std::stoi(split[8]));  //texture
        ingredient.push_back(std::stoi(split[10])); //calories
        ingredients.push_back(ingredient);
    }
    return ingredients;
};

template<bool withCals> //for task 2, true if check of 500 cals is needed for validity
auto getOptimalScore = [](const auto& ingredients){
    auto maxScore = 0l;

    std::vector<int> ingrAmount(4);
    for(ingrAmount[0] = 0u; ingrAmount[0] < 100                            ; ingrAmount[0]++){
    for(ingrAmount[1] = 0u; ingrAmount[1] < 100-ingrAmount[0]              ; ingrAmount[1]++){
    for(ingrAmount[2] = 0u; ingrAmount[2] < 100-ingrAmount[0]-ingrAmount[1]; ingrAmount[2]++){
        ingrAmount[3] = 100-ingrAmount[0]-ingrAmount[1]-ingrAmount[2];

        auto totalSize = 4u;
        if constexpr (withCals){
            totalSize = 5u;
        }
        std::vector<long> total(totalSize, 0);
        for(auto j=0u; j<total.size(); j++){
            for(auto i=0u; i<ingrAmount.size(); i++){ //j iterates over the properties of the ingredient, i over the ingredients
                total[j] += ingrAmount[i]*ingredients[i][j];
            }
            total[j] = std::max(0l, total[j]);
        }

        const auto score = total[0]*total[1]*total[2]*total[3];
        if constexpr (withCals){ //tasks fork here
            if(score > maxScore && total[4] == 500){
                maxScore=score;
            }
        }
        else{
            if(score > maxScore){
                maxScore=score;
            }
        }
    }
    }
    }

    return maxScore;
};

int main(){
    const auto ingredients = getIngredients(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto score1 = getOptimalScore<false>(ingredients);
    std::cout << "The maximum score is " << score1 << ".\n";

    //Task 2
    const auto score2 = getOptimalScore<true>(ingredients);
    std::cout << "The maximum score with 500 cal per cookie is " << score2 << ".\n";

    VerifySolution::verifySolution(score1, score2);
}