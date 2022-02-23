
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <set>
#include <map>

struct IngredientList{
    std::set<std::string> ingredients{};
    std::vector<std::string> allergens{};
};

auto parseInput(const auto& input){
    auto ingredientLists = std::vector<IngredientList>{};
    for (auto& s : input){
        const auto bracketIt = std::ranges::find(s, '(');
        const auto ingre = Utilities::split(std::begin(s), bracketIt, ' ');
        const auto aller = Utilities::splitOnEach(bracketIt + 10, std::end(s), ", )");


        auto ingreSet = std::set<std::string>{};
        std::ranges::move(ingre, std::inserter(ingreSet, std::begin(ingreSet)));
        ingredientLists.emplace_back(std::move(ingreSet), std::move(aller));
    }
    return ingredientLists;
}

template<class Set>
auto getIntersection(const Set& set1, const Set& set2){
    auto intersection = Set{};
    std::ranges::set_intersection(set1, set2, std::inserter(intersection, std::begin(intersection)));
    return intersection;
}

auto fillPossibleIngredients(const auto& ingredientLists){
    std::map<std::string, std::set<std::string>> possibleIngredients;
    for( const auto& [ingredients, allergens] : ingredientLists ){
        for( const auto& allergen : allergens ){
            if( possibleIngredients.contains(allergen) ){
                possibleIngredients[allergen] = getIntersection(possibleIngredients[allergen], ingredients);
            }
            else{
                possibleIngredients[allergen] = ingredients;
            }
        }
    }
    return possibleIngredients;
}

auto getAllergenToIngredientMap(const auto& ingredientLists){
    auto possibleIngredients = fillPossibleIngredients(ingredientLists);

    std::map<std::string, std::string> allergenToIngredient;
    auto size = possibleIngredients.size();

    do {
        std::set<std::string> ingredientsToRemove;
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

auto getDangerousIngredients(auto& allergenToIngredient){
    auto dangerousIngredients = std::string{};
    for (auto& [a, i] : allergenToIngredient){
        dangerousIngredients+= i + ",";
    }
    dangerousIngredients.pop_back();
    return dangerousIngredients;
}

int main(){
    auto ingredientLists = parseInput(readFile::vectorOfStrings());

    //Task 1
    auto allergenToIngredient = getAllergenToIngredientMap(ingredientLists);
    const auto numberOfAllergenFreeIngredients = countAllergentFreeIngredients(ingredientLists, allergenToIngredient);
    std::cout << "Allergen free ingredients appear " << numberOfAllergenFreeIngredients << " times.\n";

    //Task 2
    const auto dangerousIngredients = getDangerousIngredients(allergenToIngredient);
    std::cout << "The ingredients containing allergens are " << dangerousIngredients << ".\n";

    VerifySolution::verifySolution(numberOfAllergenFreeIngredients, dangerousIngredients);
}
