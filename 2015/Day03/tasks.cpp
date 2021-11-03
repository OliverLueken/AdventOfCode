
#include "../../readFile.h"

#include <algorithm>
#include <unordered_set>
#include <iostream>


using Position = std::pair<int,int>;
struct positionHash{
    std::size_t operator()(const Position& position) const noexcept{
        return (size_t) position.first << 32 | position.second;
    }
};

int main(){
    auto input = readFile::string("input.txt");

    Position position = std::make_pair(0,0);
    std::unordered_set<Position, positionHash> housesWithPresents;

    auto move = [&](char direction){
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

    auto deliver = [&](const Position& position){
        housesWithPresents.insert(position);
    };

    auto moveAndDeliver = [&](char direction){
        move(direction);
        deliver(position);
    };

    //Task 1
    deliver(position); //Deliver before moving
    std::ranges::for_each(input, moveAndDeliver);

    std::cout << housesWithPresents.size() << " houses receive at least one present.\n";

    //Task 2
    int i=0;
    auto even = [&](const auto&){return 0 == i++%2;};
    auto odd = [&](const auto&){return 0 == ++i%2;};

    housesWithPresents.clear();

    //Deliver with Santa
    position = std::make_pair(0,0);
    deliver(position);
    std::ranges::for_each(input | std::views::filter(even), moveAndDeliver);

    //Deliver with Robo-Santa
    i=0;
    position = std::make_pair(0,0);
    std::ranges::for_each(input | std::views::filter(odd), moveAndDeliver);

    std::cout << "With the help of Robo-Santa, " << housesWithPresents.size() << " houses receive at least one present.\n";

}