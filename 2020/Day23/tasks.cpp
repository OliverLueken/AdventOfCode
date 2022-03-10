
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <unordered_set>

class Node{
   public:
    unsigned int n;
    Node* next{nullptr};
};

class Circle{
    unsigned int maxvalue;
    // std::vector<Node*> nodepos;
    std::unordered_map<unsigned int, Node> nodeMap{};

   public:
    Node* current;
    unsigned int currentLabel;

    Circle(const std::vector<unsigned int>& s);
    Circle(const std::vector<unsigned int>& s, const unsigned int);
    
    void doNMoves(int n);
    void doOneMove();
    Node* removeNextThree();
    bool isInCircle(Node* d);
    Node* getDestinationCup(Node*);
    Node* findCup(int n);
    void insertCups(Node* cups, Node* destination);
    auto getCupNumbers(auto amount = 9u);
    auto insert(const std::vector<unsigned int>&, Node**);
    auto print() const{
        std::cout << current->n << ' ';
        for(auto nodePtr = current->next; nodePtr!=current;){
            std::cout << nodePtr->n << ' ';
            nodePtr = nodePtr->next;
        }
        std::cout << "\n";
    }
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
    auto lastInsertedElementPtr = insert(s, &current);
    lastInsertedElementPtr->next = current;

    maxvalue = std::ranges::max(nodeMap | std::views::values | std::views::transform(&Node::n));
}

Circle::Circle(const std::vector<unsigned int>& s, const unsigned int circleSize){
    nodeMap.reserve(circleSize);
    auto lastInsertedElementPtr = insert(s, &current);
    auto nextElements = std::vector<unsigned int>{10,11,12,13};
    insert(nextElements, &lastInsertedElementPtr->next);
    lastInsertedElementPtr->next = current;
    maxvalue = std::ranges::max(nodeMap | std::views::values | std::views::transform(&Node::n));
}

void Circle::doNMoves(int n){
    for(int i = 0; i < n; i++){
        doOneMove();
        print();
    }
}

void Circle::doOneMove(){
    Node* removedCups = removeNextThree();
    Node* destinationCup = getDestinationCup(removedCups);
    insertCups(removedCups, destinationCup);
    current = current->next;
}

Node* Circle::removeNextThree(){
    Node* removedCups = current->next;
    current->next = removedCups->next->next->next;
    removedCups->next->next->next = nullptr;
    return removedCups;
}

bool Circle::isInCircle(Node* d){
    if(d->next == nullptr) return false;
    d = d->next;
    if(d->next == nullptr) return false;
    d = d->next;
    if(d->next == nullptr) return false;
    return true;
}

Node* Circle::getDestinationCup(Node* removedCups){
    auto removedLabels=std::unordered_set<unsigned int>{};
    for(auto nodePtr = removedCups; nodePtr!=nullptr;){
        removedLabels.insert(nodePtr->n);
        nodePtr=nodePtr->next;
    }
    int n = current->n-1;
    while( removedLabels.contains((n+maxvalue-1)%maxvalue+1) ){
        --n;
    }
    return &nodeMap[(n+maxvalue-1)%maxvalue+1];
}

Node* Circle::findCup(int n){
    return &nodeMap[n];
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
    // c.doOneMove();
    c.doNMoves(100);
    // const auto numbers = c.getCupNumbers(s.size()-1);
    // auto number = std::string{};
    // for(const auto& i : numbers){
    //     number+= std::to_string(i);
    // }
    // return number;
    return 0;
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
