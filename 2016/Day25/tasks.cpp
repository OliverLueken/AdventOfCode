
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

auto getStartingParameter = [](const auto& input){
    const auto x = std::stoi(Utilities::split(input[1])[1]);
    const auto y = std::stoi(Utilities::split(input[2])[1]);
    const auto p = x*y;

    //We look for the first number greater than p which, continuously divided by 2, has an alternating remainder of 0 and 1
    int n=0;
    while(n<p){
        n<<=1;
        n++;
        n<<=1;
    }

    return n-p;
};

int main(){
    const auto input = readFile::vectorOfStrings("input.txt");

    const auto startingParameter = getStartingParameter(input);
    std::cout << startingParameter << '\n';

    VerifySolution::verifySolution(startingParameter);
}