
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <unordered_set>

class Cup{
   public:
    unsigned int cupLabel;
    Cup* next{nullptr};
};

class Circle{
    unsigned int maxvalue;
    std::unordered_map<unsigned int, Cup> nodeMap{};

   public:
    Cup* current;
    unsigned int currentLabel;

    Circle(const std::vector<unsigned int>& s);
    Circle(const std::vector<unsigned int>& s, const unsigned int);

    void doNMoves(int n);
    void doOneMove();
    Cup* removeNextThree();
    Cup* getDestinationCup(Cup*);
    void insertCups(Cup* cups, Cup* destination);
    auto getCupNumbers(auto amount = 9u);
    auto insert(const auto&, Cup**);
    auto print() const{
        std::cout << current->cupLabel << ' ';
        for(auto nodePtr = current->next; nodePtr!=current;){
            std::cout << nodePtr->cupLabel << ' ';
            nodePtr = nodePtr->next;
        }
        std::cout << "\n";
    }
};

auto Circle::insert(const auto& elements, Cup** start){
    auto addVal = [](auto& map, const auto val){
        return &map.emplace(val, val).first->second;
    };

    auto insertedElementPtr = addVal(nodeMap, elements[0]);
    auto lastInsertedElementPtr = insertedElementPtr;
    *start = insertedElementPtr;

    for(auto val : elements | std::views::drop(1))
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

void Circle::doNMoves(int n){
    for(int i = 0; i < n; i++){
        doOneMove();
    }
}

void Circle::doOneMove(){
    Cup* removedCups = removeNextThree();
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

Cup* Circle::getDestinationCup(Cup* removedCups){
    auto removedLabels=std::unordered_set<unsigned int>{};
    for(auto nodePtr = removedCups; nodePtr!=nullptr;){
        removedLabels.insert(nodePtr->cupLabel);
        nodePtr=nodePtr->next;
    }
    int n = current->cupLabel-1;
    while( removedLabels.contains((n+maxvalue-1)%maxvalue+1) ){
        --n;
    }
    return &nodeMap[(n+maxvalue-1)%maxvalue+1];
}

void Circle::insertCups(Cup* cups, Cup* destination){
    cups->next->next->next = destination->next;
    destination->next = cups;
}

auto Circle::getCupNumbers(const auto amount){
    auto firstCup = nodeMap[1].next;
    auto numbers = std::vector<int>{};
    for(auto i=0u; i<amount; ++i){
        numbers.push_back(firstCup->cupLabel);
        firstCup = firstCup->next;
    }
    return numbers;
}

auto playFirstGame(const auto& s){
    Circle c(s);
    c.doNMoves(100);
    const auto numbers = c.getCupNumbers(s.size()-1);
    auto number = std::string{};
    for(const auto& i : numbers){
        number+= std::to_string(i);
    }
    return number;
}

auto playSecondGame(const auto& s){
    Circle c2(s, 1'000'000);
    c2.doNMoves(10'000'000);
    const auto numbers = c2.getCupNumbers(2u);
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
