
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

auto countEscapedCharacters = [](const auto& s){
    auto escapedCharacters = 0u;
    auto it = std::ranges::find(s, '\\')+1;
    while( it < std::end(s) ){
        escapedCharacters += (*it == 'x' ? 3 : 1);
        if(*it == '\\') it++;
        it = std::ranges::find(it, std::end(s), '\\')+1;
    }
    return escapedCharacters + 2; //+2 for the surrounding ""
};

auto encodeCharacters = [](const auto& s){
    auto needsEscaping = [](const auto& c){
        return c == '\\' || c == '\"';
    };
    return std::ranges::count_if(s, needsEscaping) + 2; //+2 for the outside ""
};

int main(){
    const auto strings = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto escapedCharacters = Utilities::sum( strings, 0u, countEscapedCharacters );
    std::cout << "The answer for task 1 is " << escapedCharacters << '\n';

    //Task 2
    const auto encodedSize = Utilities::sum( strings, 0u, encodeCharacters );
    std::cout << "The answer for task 2 is " << encodedSize << '\n';

    VerifySolution::verifySolution(escapedCharacters, encodedSize);
}