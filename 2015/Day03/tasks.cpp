
#include "../../lib/readFile.hpp"

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
    const auto input = readFile::string("input.txt");

    std::unordered_set<Position, positionHash> housesWithPresents;

    auto deliverPresents = [&housesWithPresents](auto&& range) {
        auto position = std::make_pair(0,0);

        auto move = [&position](char direction){
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

        auto moveAndDeliver = [&position, &housesWithPresents, &move](char direction){
            move(direction);
            housesWithPresents.insert(position);
        };

        housesWithPresents.insert(position); //Deliver to first house (0,0) before moving
        std::ranges::for_each(range, moveAndDeliver);
    };


    //Task 1
    deliverPresents(input);
    std::cout << housesWithPresents.size() << " houses receive at least one present.\n";


    //Task 2
    auto even = [i=0](const auto&) mutable {return 0 == i++%2;};
    auto odd  = [i=1](const auto&) mutable {return 0 == i++%2;};

    housesWithPresents.clear();

    //Deliver with Santa
    deliverPresents(input | std::views::filter(even));

    //Deliver with Robo-Santa
    deliverPresents(input | std::views::filter(odd));

    std::cout << "With the help of Robo-Santa, " << housesWithPresents.size() << " houses receive at least one present.\n";

}