
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>

// typedef std::set<std::string> strset;
typedef std::vector<std::unordered_set<std::string>> strsetvec;
typedef std::vector<std::string> strvec;
typedef std::pair<int, int> point;

void readInput(const strvec& input, strsetvec& ingredients, std::vector<strvec>& allergens){
    std::string in, al;
    size_t a;
    for (auto& s : input){
        a = s.find("(");
        in = s.substr(0, a - 1);
        al = s.substr(a + 10, s.size() - a - 11);
        auto ingre = std::unordered_set<std::string>{};
        strvec aller;
        boost::split(ingre, in, boost::is_any_of(" "));
        boost::split(aller, al, boost::is_any_of(","));

        for (auto& str : aller){
            boost::trim(str);
        }
        ingredients.push_back(ingre);
        allergens.push_back(aller);
    }
}

std::map<std::string, std::string> getAllergenToIngredientMap(
    const strsetvec& ingredients, const std::vector<strvec>& allergens){
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

auto countAllergentFreeIngredients(const strsetvec& ingredients, const std::map<std::string, std::string>& allergenToIngredient){
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

auto doStuff(strvec& input){
    strsetvec ingredients;
    std::vector<strvec> allergens;
    readInput(input, ingredients, allergens);
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
