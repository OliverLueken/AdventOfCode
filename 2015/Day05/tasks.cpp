
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <functional>


auto isTaskOneNice = [](const auto& string){
    auto hasThreeVowels = [](const auto& s){
        const std::regex threeVowels{ ".*[aeiou].*[aeiou].*[aeiou].*" };
        return std::regex_search(s, threeVowels);
    };

    auto hasLetterTwiceInARow = [](const auto& s){
        return std::ranges::adjacent_find(s) != std::end(s);
    };

    auto hasNoEvilParts = [](const auto& s){
        const std::vector<std::string> badParts = { "ab", "cd", "pq", "xy"};

        auto isInsideString = [&s](const auto& badPart){
            const auto found = std::ranges::search(s, badPart);
            return !found.empty();
        };

        return std::ranges::none_of(badParts, isInsideString);
    };

    return hasThreeVowels(string) && hasLetterTwiceInARow(string) && hasNoEvilParts(string);
};


auto isTaskTwoNice = [](const auto& string){
    auto hasTwoLettersRepeating = [](const auto& s){
        for(auto it = std::begin(s); it != std::end(s); it++){
            const auto found = std::search(it+2, std::end(s), it, it+2);
            if(found != std::end(s)) return true;
        }
        return false;
    };

    auto hasLetterAfterAnotherAgain = [](const auto& s){
        //looking for the first occurances where two characters "mismatch to be not the same"
        //                     which means where two characters are the same
        const auto its = std::ranges::mismatch(std::begin(s)  , std::end(s),
                                               std::begin(s)+2, std::end(s),
                                               std::not_equal_to<char>());
        return its.in2 != std::end(s);
    };

    return hasTwoLettersRepeating(string) && hasLetterAfterAnotherAgain(string);
};


int main(){
    const auto strings = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto niceStrings1 = std::ranges::count_if(strings, isTaskOneNice);
    std::cout << "There are " << niceStrings1 << " nice strings.\n";

    //Task 2
    const auto niceStrings2 = std::ranges::count_if(strings, isTaskTwoNice);
    std::cout << "There are " << niceStrings2 << " nice strings with the new rules.\n";

    VerifySolution::verifySolution(niceStrings1, niceStrings2);
}