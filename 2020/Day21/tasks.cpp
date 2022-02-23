
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

template<class Set>
auto getSetDifference(const Set& set1, const Set& set2){
    auto setDifference = Set{};
    std::ranges::set_difference(set1, set2, std::inserter(setDifference, std::begin(setDifference)));
    return setDifference;
}

auto getAllergenToIngredientMap(const auto& ingredientLists){
    auto possibleIngredientsForAllergens = fillPossibleIngredients(ingredientLists);

    auto allergenToIngredient = std::map<std::string, std::string>{};

    auto collectDeterminedIngredientsWhichHaveAllergens = [&allergenToIngredient](auto& possibleIngredientsForAllergens_){
        auto determinedIngredientsWhichHaveAllergens = std::set<std::string>{};
        auto hasOnlyOnePossibleIngredient = [&allergenToIngredient, &determinedIngredientsWhichHaveAllergens]
            (const auto& allergenAndPossibleIngredientsForAllergenPair)
        {
            const auto& [allergen, possibleIngredientsForAllergen] = allergenAndPossibleIngredientsForAllergenPair;
            if(possibleIngredientsForAllergen.size() == 1){
                auto ingredient = *possibleIngredientsForAllergen.begin();
                allergenToIngredient[allergen] = ingredient;
                determinedIngredientsWhichHaveAllergens.emplace(std::move(ingredient));
                return true;
            }
            return false;
        };

        std::erase_if(possibleIngredientsForAllergens_, hasOnlyOnePossibleIngredient);
        return determinedIngredientsWhichHaveAllergens;
    };

    auto removeDeterminedIngredientsFromPossibleIngredients = [](auto& possibleIngredients, const auto& determinedIngredients){
        for( auto& [_, possibleIngredientsForAllergen] : possibleIngredients ){
            possibleIngredientsForAllergen = getSetDifference(possibleIngredientsForAllergen, determinedIngredients);
        }
    };

    const auto numberOfAllergens = possibleIngredientsForAllergens.size();
    do {
        const auto determinedIngredientsWhichHaveAllergens = collectDeterminedIngredientsWhichHaveAllergens(possibleIngredientsForAllergens);
        removeDeterminedIngredientsFromPossibleIngredients(possibleIngredientsForAllergens, determinedIngredientsWhichHaveAllergens);
    } while(allergenToIngredient.size() < numberOfAllergens);

    return allergenToIngredient;
}

auto countAllergenFreeIngredients(const auto& ingredientLists, const auto& allergenToIngredient){
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

auto getDangerousIngredients(const auto& allergenToIngredient){
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
    const auto allergenToIngredient = getAllergenToIngredientMap(ingredientLists);
    const auto numberOfAllergenFreeIngredients = countAllergenFreeIngredients(ingredientLists, allergenToIngredient);
    std::cout << "Allergen free ingredients appear " << numberOfAllergenFreeIngredients << " times.\n";

    //Task 2
    const auto dangerousIngredients = getDangerousIngredients(allergenToIngredient);
    std::cout << "The ingredients containing allergens are " << dangerousIngredients << ".\n";

    VerifySolution::verifySolution(numberOfAllergenFreeIngredients, dangerousIngredients);
}
