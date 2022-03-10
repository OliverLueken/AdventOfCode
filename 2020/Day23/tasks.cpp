
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <unordered_map>

struct Cup{
    unsigned int cupLabel;
    Cup* next{nullptr};
};

class Circle{
    unsigned int maxvalue;
    std::unordered_map<unsigned int, Cup> nodeMap{};
    Cup* current;

    void doOneMove();
    Cup* removeNextThree();
    Cup* getDestinationCup(Cup const* const);
    void insertCups(Cup* const cups, Cup* const destination);
    auto insert(const auto&, Cup**);
    bool labelWasRemoved(const unsigned int, const Cup*) const;

   public:
    Circle(const std::vector<unsigned int>& s);
    Circle(const std::vector<unsigned int>& s, const unsigned int);

    void doNMoves(const int);
    auto getCupsAfterCupNumberOne(const auto) const;
};

auto Circle::insert(const auto& elements, Cup** start){
    auto addVal = [](auto& map, const auto val){
        return &map.emplace(val, val).first->second;
    };

    auto insertedElementPtr = addVal(nodeMap, elements[0]);
    auto lastInsertedElementPtr = insertedElementPtr;
    *start = insertedElementPtr;

    for(const auto& val : elements | std::views::drop(1))
    {
        insertedElementPtr = addVal(nodeMap, val);
        lastInsertedElementPtr->next = insertedElementPtr;
        lastInsertedElementPtr = insertedElementPtr;
    }
    return lastInsertedElementPtr;
}

Circle::Circle(const std::vector<unsigned int>& s){
    nodeMap.reserve(s.size());
    auto lastInsertedElementPtr = insert(s, &current);
    lastInsertedElementPtr->next = current;

    maxvalue = std::ranges::max(nodeMap | std::views::values | std::views::transform(&Cup::cupLabel));
}

Circle::Circle(const std::vector<unsigned int>& s, const unsigned int circleSize){
    nodeMap.reserve(circleSize);
    auto lastInsertedElementPtr = insert(s, &current);
    maxvalue = std::ranges::max(nodeMap | std::views::values | std::views::transform(&Cup::cupLabel));
    auto nextElements = std::ranges::iota_view{maxvalue+1, circleSize+1};
    lastInsertedElementPtr = insert(nextElements, &lastInsertedElementPtr->next);
    lastInsertedElementPtr->next = current;
    maxvalue = circleSize;
}

void Circle::doNMoves(const int n){
    for(auto i = 0; i < n; i++){
        doOneMove();
    }
}

void Circle::doOneMove(){
    Cup* removedCups    = removeNextThree();
    Cup* destinationCup = getDestinationCup(removedCups);
    insertCups(removedCups, destinationCup);
    current = current->next;
}

Cup* Circle::removeNextThree(){
    Cup* removedCups = current->next;
    current->next = removedCups->next->next->next;
    removedCups->next->next->next = nullptr;
    return removedCups;
}

bool Circle::labelWasRemoved(const unsigned int label, const Cup* removedCups) const {
    while(removedCups!=nullptr){
        if(label == removedCups->cupLabel) return true;
        removedCups=removedCups->next;
    }
    return false;
}

Cup* Circle::getDestinationCup(Cup const* const removedCups){
    auto n = current->cupLabel+maxvalue-1;
    while( labelWasRemoved((n-1)%maxvalue+1, removedCups) ){
        --n;
    }
    return &nodeMap[(n-1)%maxvalue+1];
}

void Circle::insertCups(Cup* const cups, Cup* const destination){
    cups->next->next->next = destination->next;
    destination->next = cups;
}

auto Circle::getCupsAfterCupNumberOne(const auto amount) const{
    auto firstCup = nodeMap.at(1).next;
    auto numbers = std::vector<int>{};
    for(auto i=0u; i<amount; ++i){
        numbers.push_back(firstCup->cupLabel);
        firstCup = firstCup->next;
    }
    return numbers;
}

auto playFirstGame(const auto& s){
    auto c = Circle{s};
    c.doNMoves(100);
    const auto numbers = c.getCupsAfterCupNumberOne(s.size()-1);
    auto number = std::string{};
    for(const auto& i : numbers){
        number+= std::to_string(i);
    }
    return number;
}

auto playSecondGame(const auto& s){
    auto c = Circle{s, 1'000'000};
    c.doNMoves(10'000'000);
    const auto numbers = c.getCupsAfterCupNumberOne(2u);
    return (long)numbers[0]*numbers[1];
}

int main(){
    const auto input = std::vector<unsigned int>{1,3,5,4,6,8,7,2,9};

    //Task 1
    const auto cupLabelsAfterCupOne = playFirstGame(input);
    std::cout << "The cups after cup 1 are " << cupLabelsAfterCupOne << ".\n";

    //Task 2
    const auto productOfNextTwoCupLabelsAfterCupOne = playSecondGame(input);
    std::cout << "The product of the next two cups after cup 1 is " << productOfNextTwoCupLabelsAfterCupOne << ".\n";

    VerifySolution::verifySolution(cupLabelsAfterCupOne, productOfNextTwoCupLabelsAfterCupOne);
}
