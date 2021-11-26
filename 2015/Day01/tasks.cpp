
#include <string>
#include <iostream>
#include <algorithm>

#include "../../lib/readFile.hpp"

int main(){
    auto input = readFile::string("input.txt");

    //Task 1
    auto lastFloor = input.size() - 2*std::ranges::count(input, ')');
    std::cout << "Task 1: The right floor is #" << lastFloor << ".\n";

    //Task 2
    auto currentFloor = 0;
    auto enteredBasement = [&currentFloor](auto c){
        c == ')' ? currentFloor-- : currentFloor++;
        return currentFloor < 0;
    };
    auto it = std::ranges::find_if(input, enteredBasement);
    std::cout << "Task 2: Position of Santa entering the basement is " << std::distance(std::begin(input), it)+1 << ".\n";
}
