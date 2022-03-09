
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>

class Node{
   public:
    unsigned int n;
    Node* next{nullptr};
};

class Circle{
    unsigned int maxvalue;
    std::vector<Node*> nodepos;
    std::unordered_map<unsigned int, Node> nodeMap{};

   public:
    Node* current;
    unsigned int currentLabel;

    Circle(const std::vector<unsigned int>& s);
    Circle(const std::vector<unsigned int>& s, const unsigned int);
    // ~Circle(){
    //     for(auto Node : nodepos){
    //         delete Node;
    //     }
    // }
    void doNMoves(int n);
    void doOneMove();
    Node* removeNextThree();
    bool isInCircle(Node* d);
    Node* getDestinationCup();
    Node* findCup(int n);
    void insertCups(Node* cups, Node* destination);
    auto getCupNumbers(auto amount = 9u);
    auto insert(const std::vector<unsigned int>&, Node**);
};

auto Circle::insert(const std::vector<unsigned int>& elements, Node** start){
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
    insert(s, &current);
}

Circle::Circle(const std::vector<unsigned int>& s, const unsigned int circleSize){
    nodepos.resize(circleSize);
    auto lastInsertedElementPtr = insert(s, &current);
    auto nextElements = std::vector<unsigned int>{10,11,12,13};
    insert(nextElements, &lastInsertedElementPtr->next);
}

void Circle::doNMoves(int n){
    for(int i = 0; i < n; i++){
        doOneMove();
    }
}

void Circle::doOneMove(){
    Node* removedCups = removeNextThree();
    Node* destinationCup = getDestinationCup();
    insertCups(removedCups, destinationCup);
    current = current->next;
}

Node* Circle::removeNextThree(){
    Node* removedCups = current->next;
    current->next = removedCups->next->next->next;
    removedCups->next->next->next = NULL;
    return removedCups;
}

bool Circle::isInCircle(Node* d){
    if(d->next == NULL) return false;
    d = d->next;
    if(d->next == NULL) return false;
    d = d->next;
    if(d->next == NULL) return false;
    return true;
}

Node* Circle::getDestinationCup(){
    int n = current->n;
    Node* destinationCup;
    bool isInC;
    do {
        n = n + maxvalue - 2;
        n = n % maxvalue + 1;
        destinationCup = findCup(n);
        isInC = isInCircle(destinationCup);

    } while (!isInC);
    return destinationCup;
}

Node* Circle::findCup(int n){
    return nodepos[n - 1];
}

void Circle::insertCups(Node* cups, Node* destination){
    cups->next->next->next = destination->next;
    destination->next = cups;
}

auto Circle::getCupNumbers(const auto amount){
    auto n = findCup(1)->next;
    auto numbers = std::vector<int>{};
    for(auto i=0u; i<amount; ++i){
        numbers.push_back(n->n);
        n = n->next;
    }
    return numbers;
}

auto playFirstGame(const auto& s){
    Circle c(s);

    // c.doNMoves(100);
    // const auto numbers = c.getCupNumbers(s.size()-1);
    // auto number = std::string{};
    // for(const auto& i : numbers){
    //     number+= std::to_string(i);
    // }
    // return number;
}

auto playSecondGame(const auto& s){
    Circle c2(s, 1'000'000);
    // c2.doNMoves(10'000'000);
    // const auto numbers = c2.getCupNumbers(2u);
    // return (long)numbers[0]*numbers[1];
    return 0;
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
