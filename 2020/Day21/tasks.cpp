
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>

struct IngredientList{
    std::unordered_set<std::string> ingredients{};
    std::vector<std::string> allergens{};
};

auto parseInput(const auto& input){
    auto ingredientLists = std::vector<IngredientList>{};
    for (auto& s : input){
        const auto bracketIt = std::ranges::find(s, '(');
        const auto ingre = Utilities::split(std::begin(s), bracketIt, ' ');
        const auto aller = Utilities::splitOnEach(bracketIt + 10, std::end(s), ", )");


        auto ingreSet = std::unordered_set<std::string>{};
        std::ranges::move(ingre, std::inserter(ingreSet, std::begin(ingreSet)));
        ingredientLists.emplace_back(std::move(ingreSet), std::move(aller));
    }
    return ingredientLists;
}

auto getAllergenToIngredientMap(const auto& ingredientLists){
    std::map<std::string, std::unordered_set<std::string>> possibleIngredients;

    for (auto i = 0u; i < ingredientLists.size(); i++){
        auto& ingredients = ingredientLists[i].ingredients;
        auto& allergens   = ingredientLists[i].allergens;
        for (auto& allergen : allergens){
            if (possibleIngredients.find(allergen) ==
                possibleIngredients.end()){
                possibleIngredients[allergen] = ingredients;
            } else {
                std::unordered_set<std::string> tempingr = ingredients;
                tempingr.merge(possibleIngredients[allergen]);
            }
        }
    }

    std::map<std::string, std::string> allergenToIngredient;
    auto size = possibleIngredients.size();

    do {
        std::unordered_set<std::string> ingredientsToRemove;
        for (auto it = possibleIngredients.begin();
             it != possibleIngredients.end();){
            if (it->second.size() == 1){
                std::string ingredient = *(it->second.begin());
                ingredientsToRemove.insert(ingredient);
                allergenToIngredient[it->first] = ingredient;
                it = possibleIngredients.erase(it);
            } else {
                it++;
            }
        }

        for (auto it = possibleIngredients.begin();
             it != possibleIngredients.end(); it++){
            for (auto ingredient : ingredientsToRemove){
                it->second.erase(ingredient);
            }
        }
    } while (allergenToIngredient.size() < size);

    return allergenToIngredient;
}

auto countAllergentFreeIngredients(const auto& ingredientLists, const auto& allergenToIngredient){
    auto numberOfAllergenFreeIngredients = 0u;
    for (auto& ingredientList : ingredientLists){
        auto& ingset = ingredientList.ingredients;
        for (auto& ingredient : ingset){
            auto isAllergenFree = true;
            for (auto& [a, i] : allergenToIngredient)
                if (ingredient == i) isAllergenFree = false;
            if (isAllergenFree) numberOfAllergenFreeIngredients++;
        }
    }
    return numberOfAllergenFreeIngredients;
}

auto doStuff(auto& ingredientLists){
    auto allergenToIngredient = getAllergenToIngredientMap(ingredientLists);

    const auto numberOfAllergenFreeIngredients = countAllergentFreeIngredients(ingredientLists, allergenToIngredient);

    auto dangerousIngredients = std::string{};
    for (auto& [a, i] : allergenToIngredient){
        dangerousIngredients+= i + ",";
    }
    dangerousIngredients.pop_back();
    return std::make_pair(numberOfAllergenFreeIngredients, dangerousIngredients);
}

int main(){
    auto ingredientLists = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto [numberOfAllergenFreeIngredients, dangerousIngredients] = doStuff(ingredientLists);
    std::cout << "Allergen free ingredients appear " << numberOfAllergenFreeIngredients << " times.\n";

    //Task 2
    std::cout << "The ingredients containing allergens are " << dangerousIngredients << ".\n";

    VerifySolution::verifySolution(numberOfAllergenFreeIngredients, dangerousIngredients);
}
