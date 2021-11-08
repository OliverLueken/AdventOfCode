
#include "../../readFile.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <functional>


auto isTaskOneNice = [](const auto& string){
    auto hasThreeVowels = [](const auto& string){
        const std::regex threeVowels{ ".*[aeiou].*[aeiou].*[aeiou].*" };
        return std::regex_search(string, threeVowels);
    };

    auto hasLetterTwiceInARow = [](const auto& string){
        return std::ranges::adjacent_find(string) != std::end(string);
    };

    auto hasNoEvilParts = [](const auto& string){
        const std::vector<std::string> badParts = { "ab", "cd", "pq", "xy"};

        auto isInsideString = [&string](const auto& badPart){
            const auto found = std::ranges::search(string, badPart);
            return !found.empty();
        };

        return std::ranges::none_of(badParts, isInsideString);
    };

    return hasThreeVowels(string) && hasLetterTwiceInARow(string) && hasNoEvilParts(string);
};


auto isTaskTwoNice = [](const auto& string){
    auto hasTwoLettersRepeating = [](const auto& string){
        for(auto it = std::begin(string); it != std::end(string); it++){
            const auto found = std::search(it+2, std::end(string), it, it+2);
            if(found != std::end(string)) return true;
        }
        return false;
    };

    auto hasLetterAfterAnotherAgain = [](const auto& string){
        //looking for the first occurances where two characters "mismatch to be not the same"
        //                     which means where two characters are the same
        const auto its = std::ranges::mismatch(std::begin(string)  , std::end(string),
                                               std::begin(string)+2, std::end(string),
                                               std::not_equal_to<char>());
        return its.in2 != std::end(string);
    };

    return hasTwoLettersRepeating(string) && hasLetterAfterAnotherAgain(string);
};


int main(){
    const auto strings = readFile::vectorOfStrings("input.txt");

    //Task 1
    size_t niceStrings = std::ranges::count_if(strings, isTaskOneNice);
    std::cout << "There are " << niceStrings << " nice strings.\n";

    //Task 2
    niceStrings = std::ranges::count_if(strings, isTaskTwoNice);
    std::cout << "There are " << niceStrings << " nice strings with the new rules.\n";

}