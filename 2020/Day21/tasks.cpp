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

typedef std::set<std::string> strset;
typedef std::vector<strset> strsetvec;
typedef std::vector<std::string> strvec;
typedef std::pair<int, int> point;

unsigned long result1 = 0, result2 = 0;

void readInput(const strvec& input, strsetvec& ingredients, std::vector<strvec>& allergens){
    std::string in, al;
    size_t a;
    for (auto& s : input){
        a = s.find("(");
        in = s.substr(0, a - 1);
        al = s.substr(a + 10, s.size() - a - 11);
        std::cout << in << std::endl << al << std::endl << std::endl;
        strset ingre;
        strvec aller;
        boost::split(ingre, in, boost::is_any_of(" "));
        boost::split(aller, al, boost::is_any_of(","));

        for (auto& s : ingre) std::cout << "\"" << s << "\"" << std::endl;
        for (auto& s : aller){
            boost::trim(s);
            std::cout << "\"" << s << "\"" << std::endl;
        }
        std::cout << std::endl;
        ingredients.push_back(ingre);
        allergens.push_back(aller);
    }
}

std::map<std::string, std::string> getAllergenToIngredientMap(
    const strsetvec& ingredients, const std::vector<strvec>& allergens){
    std::map<std::string, std::set<std::string>> possibleIngredients;

    for (int i = 0; i < allergens.size(); i++){
        for (auto& allergen : allergens.at(i)){
            if (possibleIngredients.find(allergen) ==
                possibleIngredients.end()){
                possibleIngredients[allergen] = ingredients.at(i);
            } else {
                std::set<std::string> tempingr = ingredients.at(i);
                tempingr.merge(possibleIngredients[allergen]);
            }
        }
    }

    for (auto [allergen, ingredients] : possibleIngredients){
        std::cout << allergen << std::endl;
        for (auto it = ingredients.begin(); it != ingredients.end(); it++){
            std::cout << *it << std::endl;
        }
        std::cout << std::endl;
    }

    std::map<std::string, std::string> allergenToIngredient;
    int size = possibleIngredients.size();

    do {
        std::set<std::string> ingredientsToRemove;
        for (auto it = possibleIngredients.begin();
             it != possibleIngredients.end();){
            if (it->second.size() == 1){
                std::string ingredient = *(it->second.begin());
                ingredientsToRemove.insert(ingredient);
                allergenToIngredient[it->first] = ingredient;
                std::cout << it->first << ": " << ingredient << std::endl;
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

void countAllergentFreeIngredients(
    const strsetvec& ingredients,
    const std::map<std::string, std::string>& allergenToIngredient){
    for (auto& ingset : ingredients){
        for (auto& ingredient : ingset){
            bool isAllergenFree = true;
            for (auto& [a, i] : allergenToIngredient)
                if (ingredient == i) isAllergenFree = false;
            if (isAllergenFree) result1++;
        }
    }
}

void doStuff(strvec& input){
    strsetvec ingredients;
    std::vector<strvec> allergens;
    readInput(input, ingredients, allergens);
    std::cout << "a\n";
    std::map<std::string, std::string> allergenToIngredient;
    allergenToIngredient = getAllergenToIngredientMap(ingredients, allergens);

    std::cout << "b\n";
    countAllergentFreeIngredients(ingredients, allergenToIngredient);

    for (auto& [a, i] : allergenToIngredient){
        std::cout << i << ",";
    }
}

strvec readfile(std::string file){
    std::string line;
    std::ifstream input(file);
    strvec lines;

    if (input.is_open()){
        while (getline(input, line)){
            lines.push_back(line);
        }
        input.close();
    } else {
        std::cout << "Unable to open file\n";
    }
    return lines;
}

int main(){
    strvec input = readfile("input.txt");

    doStuff(input);

    std::cout << result1 << "\n";
    std::cout << result2 << "\n";
}
