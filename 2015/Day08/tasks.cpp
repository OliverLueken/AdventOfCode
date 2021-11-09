
#include "../../readFile.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

auto countEscapedCharacters = [](const auto& s){
    size_t escapedCharacters = 0u;
    auto it = std::ranges::find(s, '\\')+1;
    while( it < std::end(s) ){
        *it == 'x' ? escapedCharacters+=3 : escapedCharacters++;
        if(*it == '\\') it++;
        it = std::find(it, std::end(s), '\\')+1;
    }
    return escapedCharacters + 2; //2 for the outside ""
};

auto encodeCharacters = [](const auto& s){
    auto needsEscaping = [](const auto& c){
        return c == '\\' || c == '\"';
    };
    return std::ranges::count_if(s, needsEscaping) + 2; //2 for the outside ""
};

int main(){
    const auto strings = readFile::vectorOfStrings("input.txt");

    //Task 1
    auto escapedCharacters = std::transform_reduce(std::begin(strings), std::end(strings), 0u,
                                                   std::plus<unsigned int>(), countEscapedCharacters);
    std::cout << "The answer for task 1 is " << escapedCharacters << '\n';

    //Task 2
    auto encodedSize = std::transform_reduce(std::begin(strings), std::end(strings), 0u,
                                                   std::plus<unsigned int>(), encodeCharacters);
    std::cout << "The answer for task 2 is " << encodedSize << '\n';
}