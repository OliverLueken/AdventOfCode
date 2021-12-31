
#include <string>
#include <iostream>
#include <algorithm>

#include "../../lib/readFile.hpp"

int main(){
    const auto input = readFile::string("input.txt");

    //Task 1
    const auto lastFloor = input.size() - 2*std::ranges::count(input, ')');
    std::cout << "Task 1: The right floor is #" << lastFloor << ".\n";

    //Task 2
    auto enteredBasement = [currentFloor = 0](const auto c) mutable {
        c == ')' ? currentFloor-- : currentFloor++;
        return currentFloor < 0;
    };
    const auto firstBasementFloorIt = std::ranges::find_if(input, enteredBasement);
    const auto basementPosition = std::distance(std::begin(input), firstBasementFloorIt)+1;
    std::cout << "Task 2: Position of Santa entering the basement is " << basementPosition << ".\n";
}
