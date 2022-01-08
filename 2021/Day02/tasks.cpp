
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>



auto getTask1 = [](const auto& input){
    auto posX = 0;
    auto posY = 0;
    for(const auto& in : input){
        const auto split = Utilities::split(in);
        const auto cmd   = split[0];
        const auto x     = std::stoi(split[1]);

        if( cmd == "down" ){
            posY += x;
        }
        else if( cmd == "up" ){
            posY -= x;
        }
        else{ //cmd == "forward"
            posX += x;
        }
    }
    return posX*posY;
};

auto getTask2 = [](const auto& input){
    auto posX = 0;
    auto posY = 0;
    auto aim  = 0;
    for(const auto& in : input){
        const auto split = Utilities::split(in);
        const auto cmd   = split[0];
        const auto x     = std::stoi(split[1]);

        if( cmd == "down" ){
            aim += x;
        }
        else if( cmd == "up" ){
            aim-=x;
        }
        else{ //cmd == "forward"
            posX += x;
            posY += x*aim;
        }
    }
    return posX*posY;
};

int main(){
    const auto input = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto taskOneResult = getTask1(input);
    std::cout << "The product of the horizontal and vertical position is " << taskOneResult << ".\n";

    //Task 2
    const auto taskTwoResult = getTask2(input);
    std::cout << "The new product of the horizontal and vertical position is " << taskTwoResult << ".\n";

    VerifySolution::verifySolution(taskOneResult, taskTwoResult);
}