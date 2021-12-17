
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <ranges>
#include <string>


using Position = Utilities::Position<int>;

auto deliverPresents = [](auto&& instructions, auto& housesWithPresents) {
    auto position = std::make_pair(0,0);

    auto move = [&position](const char direction){
        switch(direction){
            case '>':
                position.first++;
                break;
            case '<':
                position.first--;
                break;
            case '^':
                position.second++;
                break;
            case 'v':
                position.second--;
                break;
        }
    };

    auto moveAndDeliver = [&position, &housesWithPresents, &move](const char direction){
        move(direction);
        housesWithPresents.insert(position);
    };

    housesWithPresents.insert(position); //Deliver to first house (0,0) before moving
    std::ranges::for_each(instructions, moveAndDeliver);
};


auto deliverWithSanta(const auto& input){
    std::unordered_set<Position> housesWithPresents{};
    deliverPresents(input, housesWithPresents);
    return housesWithPresents.size();
}

auto deliverWithSantaAndRobot(const auto& input){
    std::unordered_set<Position> housesWithPresents{};

    auto even = [i=0](const auto&) mutable {return 0 == i++%2;};
    auto odd  = [i=1](const auto&) mutable {return 0 == i++%2;};

    deliverPresents(input | std::views::filter(even), housesWithPresents); //Deliver with Santa
    deliverPresents(input | std::views::filter(odd) , housesWithPresents); //Deliver with Robo-Santa

    return housesWithPresents.size();
}

int main(){
    const auto input = readFile::string("input.txt");

    //Task 1
    const auto numberOfHousesWithPresents = deliverWithSanta(input);
    std::cout << "Santa delivered presents to " << numberOfHousesWithPresents << " different houses.\n";


    //Task 2
    const auto numberOfHousesWithPresents2 = deliverWithSantaAndRobot(input);
    std::cout << "With the help of Robo-Santa, " << numberOfHousesWithPresents2 << " houses receive at least one present.\n";

}