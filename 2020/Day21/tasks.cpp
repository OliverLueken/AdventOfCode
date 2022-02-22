
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

auto readInput(const auto& input){
    auto ingredients = std::vector<std::unordered_set<std::string>>{};
    auto allergens   = std::vector<std::vector<std::string>>{};

    for (auto& s : input){
        const auto bracketIt = std::ranges::find(s, '(');
        const auto ingre = Utilities::split(std::begin(s), bracketIt, ' ');
        const auto aller = Utilities::splitOnEach(bracketIt + 10, std::end(s), ", )");

        auto ingreSet = std::unordered_set<std::string>{};
        std::ranges::move(ingre, std::inserter(ingreSet, std::begin(ingreSet)));
        ingredients.emplace_back(std::move(ingreSet));
        allergens.emplace_back(std::move(aller));
    }
    return std::make_pair(ingredients, allergens);
}

std::map<std::string, std::string> getAllergenToIngredientMap(
    const auto& ingredients, const auto& allergens){
    std::map<std::string, std::unordered_set<std::string>> possibleIngredients;

    for (auto i = 0u; i < allergens.size(); i++){
        for (auto& allergen : allergens.at(i)){
            if (possibleIngredients.find(allergen) ==
                possibleIngredients.end()){
                possibleIngredients[allergen] = ingredients.at(i);
            } else {
                std::unordered_set<std::string> tempingr = ingredients.at(i);
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

auto countAllergentFreeIngredients(const auto& ingredients, const std::map<std::string, std::string>& allergenToIngredient){
    auto numberOfAllergenFreeIngredients = 0u;
    for (auto& ingset : ingredients){
        for (auto& ingredient : ingset){
            bool isAllergenFree = true;
            for (auto& [a, i] : allergenToIngredient)
                if (ingredient == i) isAllergenFree = false;
            if (isAllergenFree) numberOfAllergenFreeIngredients++;
        }
    }
    return numberOfAllergenFreeIngredients;
}

auto doStuff(auto& input){
    auto [ingredients, allergens] = readInput(input);
    std::map<std::string, std::string> allergenToIngredient;
    allergenToIngredient = getAllergenToIngredientMap(ingredients, allergens);

    const auto numberOfAllergenFreeIngredients = countAllergentFreeIngredients(ingredients, allergenToIngredient);

    auto dangerousIngredients = std::string{};
    for (auto& [a, i] : allergenToIngredient){
        dangerousIngredients+= i + ",";
    }
    dangerousIngredients.pop_back();
    return std::make_pair(numberOfAllergenFreeIngredients, dangerousIngredients);
}

int main(){
    auto input = readFile::vectorOfStrings();

    //Task 1
    const auto [numberOfAllergenFreeIngredients, dangerousIngredients] = doStuff(input);
    std::cout << "Allergen free ingredients appear " << numberOfAllergenFreeIngredients << " times.\n";

    //Task 2
    std::cout << "The ingredients containing allergens are " << dangerousIngredients << ".\n";

    VerifySolution::verifySolution(numberOfAllergenFreeIngredients, dangerousIngredients);
}
