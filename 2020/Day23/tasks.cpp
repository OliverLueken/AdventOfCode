
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class node{
   public:
    unsigned int n;
    node* next;
};

class circle{
    unsigned int maxvalue;
    std::vector<node*> nodepos;

   public:
    node* current;

    circle(const std::string& s);
    circle(const std::string& s, const unsigned int);
    ~circle(){
        for(auto node : nodepos){
            delete node;
        }
    }
    void doNMoves(int n);
    void doOneMove();
    node* removeNextThree();
    bool isInCircle(node* d);
    node* getDestinationCup();
    node* findCup(int n);
    void insertCups(node* cups, node* destination);
    auto getCupNumbers(auto amount = 9u);
};

circle::circle(const std::string& s){
    nodepos.resize(9);

    node* c = new node;
    c->n = s[0] - '0';
    nodepos[c->n - 1] = c;

    maxvalue = c->n;
    current = c;

    for(auto i = 1u; i < s.size(); i++){
        node* b = new node;
        b->n = s[i] - '0';
        nodepos[b->n - 1] = b;

        if(b->n > maxvalue) maxvalue = b->n;
        c->next = b;
        c = b;
    }
    c->next = current;
}

circle::circle(const std::string& s, const unsigned int circleSize){
    nodepos.resize(10*circleSize);

    node* c = new node;
    c->n = s[0] - '0';
    nodepos[c->n - 1] = c;

    maxvalue = c->n;
    current = c;
    for(auto i = 1u; i < s.size(); i++){
        node* b = new node;
        b->n = s[i] - '0';
        nodepos[b->n - 1] = b;

        if(b->n > maxvalue) maxvalue = b->n;
        c->next = b;
        c = b;
    }
    for(auto i = maxvalue + 1; i <= circleSize; i++){
        node* b = new node;
        b->n = i;
        nodepos[b->n - 1] = b;

        c->next = b;
        c = b;
    }
    maxvalue = circleSize;

    c->next = current;
}

void circle::doNMoves(int n){
    for(int i = 0; i < n; i++){
        doOneMove();
    }
}

void circle::doOneMove(){
    node* removedCups = removeNextThree();
    node* destinationCup = getDestinationCup();
    insertCups(removedCups, destinationCup);
    current = current->next;
}

node* circle::removeNextThree(){
    node* removedCups = current->next;
    current->next = removedCups->next->next->next;
    removedCups->next->next->next = NULL;
    return removedCups;
}

bool circle::isInCircle(node* d){
    if(d->next == NULL) return false;
    d = d->next;
    if(d->next == NULL) return false;
    d = d->next;
    if(d->next == NULL) return false;
    return true;
}

node* circle::getDestinationCup(){
    int n = current->n;
    node* destinationCup;
    bool isInC;
    do {
        n = n + maxvalue - 2;
        n = n % maxvalue + 1;
        destinationCup = findCup(n);
        isInC = isInCircle(destinationCup);

    } while (!isInC);
    return destinationCup;
}

node* circle::findCup(int n){
    return nodepos[n - 1];
}

void circle::insertCups(node* cups, node* destination){
    cups->next->next->next = destination->next;
    destination->next = cups;
}

auto circle::getCupNumbers(const auto amount){
    auto n = findCup(1)->next;
    auto numbers = std::vector<int>{};
    for(auto i=0u; i<amount; ++i){
        numbers.push_back(n->n);
        n = n->next;
    }
    return numbers;
}

auto playFirstGame(const auto& s){
    circle c(s);

    c.doNMoves(100);
    const auto numbers = c.getCupNumbers(s.size()-1);
    auto number = std::string{};
    for(const auto& i : numbers){
        number+= std::to_string(i);
    }
    return number;
}

auto playSecondGame(const auto& s){
    circle c2(s, 1'000'000);
    c2.doNMoves(10'000'000);
    const auto numbers = c2.getCupNumbers(2u);
    return (long)numbers[0]*numbers[1];
}

int main(){
    const auto input = std::string{"135468729"};

    //Task 1
    const auto cupLabelsAfterCupOne = playFirstGame(input);
    std::cout << "The cups after cup 1 are " << cupLabelsAfterCupOne << ".\n";

    //Task 2
    const auto productOfNextTwoCupLabelsAfterCupOne = playSecondGame(input);
    std::cout << "The product of the next two cups after cup 1 is " << productOfNextTwoCupLabelsAfterCupOne << ".\n";

    VerifySolution::verifySolution(cupLabelsAfterCupOne, productOfNextTwoCupLabelsAfterCupOne);
}
