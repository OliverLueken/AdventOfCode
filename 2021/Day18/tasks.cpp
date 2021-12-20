
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <ranges>
#include <variant>
#include <optional>

using Position = Utilities::Position<int>;

enum Son{left, right};

template<Son son>
constexpr Son getSibling(){
    if constexpr (son == Son::left){
        return Son::right;
    }
    else{
        return Son::left;
    }
}

class Node;
class Node{
    Node* father{nullptr};
    std::variant<int, std::unique_ptr<Node>> left{};
    std::variant<int, std::unique_ptr<Node>> right{};

public:
    Node(const std::string& s, Node* father = nullptr) : father{father}{
        auto separatorPos = 0u;
        if(s[1]=='['){
            auto bracketCounter=0;
            for(auto pos=1u; pos<=s.size(); pos++){
                if(s[pos]=='[') bracketCounter++;
                if(s[pos]==']') bracketCounter--;
                if(bracketCounter==0){
                    separatorPos = pos+1;
                    break;
                }
            }
            left = std::make_unique<Node>(s.substr(1, separatorPos-1 ), this);
        }
        else{
            auto commaPos = std::ranges::find(s, ',');
            separatorPos = (unsigned int) std::distance(std::begin(s), commaPos);
            left = std::stoi(s.substr(1));
        }

        if(s[separatorPos+1]=='['){
            right = std::make_unique<Node>(s.substr(separatorPos+1  ), this);
        }
        else{
            right = std::stoi(s.substr(separatorPos+1));
        }
    }

    Node(std::unique_ptr<Node>&& left, const std::string& s, Node* father = nullptr)
        : father{father}, left{std::move(left)}, right{std::make_unique<Node>(s, this)}{
        std::get<1>(this->left)->father = this;
    }

    Node(int left, int right, Node* father) : father{father}, left{left}, right{right} {}

    constexpr auto getFather(){
        return father;
    }

    template<Son son>
    constexpr auto setSon( std::variant<int, std::unique_ptr<Node>>&& val ) {
        if constexpr (son==Son::left){
            left  = std::move(val);
        }
        else{
            right = std::move(val);
        }
    }

    template<Son son>
    constexpr auto getSon() -> std::variant<int, std::unique_ptr<Node>>& {
        if constexpr (son==Son::left){
            return left;
        }
        else{
            return right;
        }
    }

    template<Son son>
    constexpr auto getSon() const -> const std::variant<int, std::unique_ptr<Node>>& {
        if constexpr (son==Son::left){
            return left;
        }
        else{
            return right;
        }
    }

    template<Son son>
    constexpr bool isSonValue() const {
        return getSon<son>().index()==0;
    }

    template<Son son>
    constexpr bool isSonPtr() const {
        return !isSonValue<son>();
    }

    template<Son son>
    constexpr int& getSonValue() {
        return std::get<0>(getSon<son>());
    }

    template<Son son>
    constexpr int getSonValue() const {
        return std::get<0>(getSon<son>());
    }

    template<Son son>
    constexpr auto getSonPtr() const -> Node* {
        return std::get<1>(getSon<son>()).get();
    }

    template<Son son>
    constexpr bool amISon() const {
        return father->isSonPtr<son>() && this == father->getSonPtr<son>();
    }
};

template<Son son>
auto getNeighborNumber(Node* nodePtr) {
    if(nodePtr->isSonValue<son>()) return std::optional<std::reference_wrapper<int>>{nodePtr->getSonValue<son>()};
    Node* sonPtr = nodePtr->getSonPtr<son>();
    return getNeighborNumber<son>(sonPtr);
}


template<Son son>
auto getReceivingNumber(Node* nodePtr) -> std::optional<std::reference_wrapper<int>> {
    auto father = nodePtr->getFather();
    if(father == nullptr){ return std::nullopt; }
    if( nodePtr->amISon<son>() ){
        return getReceivingNumber<son>(father); //bubble up
    }
    //we are the sibling(right) of son(left), visit son(left) then go to sibling(right) direction as far as possible
    if( father->isSonValue<son>() ) return std::optional<std::reference_wrapper<int>>{father->getSonValue<son>()};
    Node* siblingPtr = father->getSonPtr<son>();
    constexpr Son sibling = getSibling<son>();
    return getNeighborNumber<sibling>(siblingPtr);
}

auto explodeNode(Node* nodePtr){
    const auto leftValue      = nodePtr->getSonValue<left>();
    const auto rightValue     = nodePtr->getSonValue<right>();
    auto leftReceivingNumber  = getReceivingNumber<left>(nodePtr);
    auto rightReceivingNumber = getReceivingNumber<right>(nodePtr);

    if( leftReceivingNumber)  leftReceivingNumber.value()+=leftValue;
    if(rightReceivingNumber) rightReceivingNumber.value()+=rightValue;

    if( nodePtr->amISon<Son::left>() ){
        nodePtr->getFather()->setSon<Son::left>(0);
    }
    else{
        nodePtr->getFather()->setSon<Son::right>(0);
    }
}

auto explode(Node* nodePtr, auto level) -> bool {
    auto didExplode = false;
    if( nodePtr->isSonPtr<Son::left>() || nodePtr->isSonPtr<Son::right>() ){
        if(nodePtr->isSonPtr<Son::left>()){
            didExplode |= explode(nodePtr->getSonPtr<Son::left>(), level+1);
        }
        if(nodePtr->isSonPtr<Son::right>()){
            didExplode |= explode(nodePtr->getSonPtr<Son::right>(), level+1);
        }
    }
    else{
        if(level>4){
            explodeNode(nodePtr);
            return true;
        }
    }
    return didExplode;
}

auto split(Node*) -> bool;

template<Son son>
auto tryToSplit(Node* nodePtr){
    if(nodePtr->isSonPtr<son>()){
        Node* sonPtr = nodePtr->getSonPtr<son>();
        return split(sonPtr);
    }

    const auto value = nodePtr->getSonValue<son>();
    if(value < 10) return false;

    nodePtr->setSon<son>( std::make_unique<Node>(
        static_cast<int>(std::floor(value/2.)),
        static_cast<int>(std::ceil (value/2.)),
        nodePtr
    ));
    return true;
}

auto split(Node* nodePtr) -> bool {
    if(    tryToSplit<Son::left >(nodePtr) ) return true;
    return tryToSplit<Son::right>(nodePtr);
}

auto explodeAndSplit = [](auto& rootPtr){
    do{
        while(explode(rootPtr.get(), 1)){}
    }while(split(rootPtr.get()));
};

auto addSnailFish(const auto& snailfishes){
    auto root = std::make_unique<Node>(snailfishes[0]);
    for(const auto& row : snailfishes | std::views::drop(1)){
        root = std::make_unique<Node>(std::move(root), row);
        explodeAndSplit(root);
    }
    return root;
}

auto getMagnitude(Node*) -> unsigned long;

template<Son son>
auto getSonMagnitude(Node* nodePtr) -> unsigned long {
    if( nodePtr->isSonValue<son>() ){
        return nodePtr->getSonValue<son>();
    }
    return getMagnitude(nodePtr->getSonPtr<son>());
}

auto getMagnitude(Node* nodePtr) -> unsigned long {
    const auto left  = getSonMagnitude<Son::left>(nodePtr);
    const auto right = getSonMagnitude<Son::right>(nodePtr);
    return 3*left+2*right;
};

auto getMaxMagnitudeOfTwoSnailfishes = [](const auto& snailfishes){
    auto maxMagnitude = 0ul;
    for(auto i=0u; i<snailfishes.size(); i++){
        for(auto j=0u; j<snailfishes.size(); j++){
            if(i!=j){
                auto root = addSnailFish(std::vector<std::string>{snailfishes[i], snailfishes[j]});
                const auto magnitude = getMagnitude(root.get());
                maxMagnitude = std::ranges::max(maxMagnitude, magnitude);
            }
        }
    }
    return maxMagnitude;
};



int main(){
    const auto snailfishes = readFile::vectorOfStrings("input.txt");
    auto root = addSnailFish(snailfishes);

    //Task 1
    const auto magnitude = getMagnitude(root.get());
    std::cout << "Task 1: " << magnitude << '\n';

    //Task 2
    const auto maxMagnitudeOfTwoSnailfishes = getMaxMagnitudeOfTwoSnailfishes(snailfishes);
    std::cout << "Task 2: " << maxMagnitudeOfTwoSnailfishes << '\n';
}