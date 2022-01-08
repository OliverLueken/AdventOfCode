
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>

auto swapPositions = [](auto& s, const std::variant<int, char> x_, const std::variant<int, char> y_, const bool){
    const int x = std::get<int>(x_);
    const int y = std::get<int>(y_);
    auto it = std::begin(s);
    std::swap(*(it+x), *(it+y));
};

auto swapLetters = [](auto& s, const std::variant<int, char> x_, const std::variant<int, char> y_, const bool){
    const char x = std::get<char>(x_);
    const char y = std::get<char>(y_);
    auto it_x = std::ranges::find(s, x);
    auto it_y = std::ranges::find(s, y);
    *it_x = y;
    *it_y = x;
};

auto rotate = [](auto& s, const std::variant<int, char> x_, const std::variant<int, char> y_, const bool reversed){
    const char x = std::get<char>(x_);
    int y = std::get<int> (y_);
    if(x == 'r') y=-y;
    if(reversed) y=-y;
    Utilities::rotate(s, y);
};

auto rotateByLetter = [](auto& s, const std::variant<int, char> x_, const std::variant<int, char>, const bool reversed){
    const char x = std::get<char>(x_);
    const auto it_x = std::ranges::find(s, x);
    auto d = std::distance(std::begin(s), it_x);
    const auto n = s.size();
    if(reversed){
        d = (d%2 != 0) ? (d+1)/2 : d+( (-d)%n -n )/2-3;
    }
    else{
        d = (d < 4) ? -d-1 : (n-d-2)%n;
    }
    Utilities::rotate(s, d);
};

auto reverse = [](auto& s, const std::variant<int, char> x_, const std::variant<int, char> y_, const bool){
    const auto x = std::get<int>(x_);
    const auto y = std::get<int>(y_);
    std::ranges::reverse(std::begin(s)+x, std::begin(s)+y+1);
};

auto move = [](auto& s, const std::variant<int, char> x_, const std::variant<int, char> y_, const bool reversed){
    const auto x = std::get<int>(x_);
    const auto y = std::get<int>(y_);
    auto direction = 1;
    if((x > y) ^ reversed) direction = -1;
    Utilities::rotate(std::begin(s)+std::min(x,y), std::begin(s)+std::max(x,y)+1, direction);
};


struct Instruction{
    std::function<void(std::string&, std::variant<int, char>, std::variant<int, char>, bool)> fun;
    std::variant<int, char> x{0};
    std::variant<int, char> y{0};

    Instruction(auto _fun, auto _x, auto _y) : fun{_fun}, x{_x}, y{_y} {}
    Instruction(auto _fun, auto _x)          : fun{_fun}, x{_x}        {}

    void operator()(std::string& s, const bool reverseInstruction = false) const {
        fun(s, x, y, reverseInstruction);
    }
};

auto parseIn = [](const auto& in, auto& instructions){
    const auto split = Utilities::split(in);
    if(      split[0] == "swap" && split[1] == "position"){
        instructions.emplace_back(swapPositions, std::stoi(split[2]), std::stoi(split[5]));
    }
    else if( split[0] == "swap" && split[1] != "position"){ //split[1] == "letter"
        instructions.emplace_back(swapLetters, split[2][0], split[5][0]);
    }
    else if( split[0] == "rotate" && split[1] == "based"){
        instructions.emplace_back(rotateByLetter, split[6][0]);
    }
    else if( split[0] == "rotate" && split[1] != "based"){ //split[1] == "left/right"
        instructions.emplace_back(rotate, split[1][0], std::stoi(split[2]));
    }
    else if( split[0] == "reverse" ){
        instructions.emplace_back(reverse, std::stoi(split[2]), std::stoi(split[4]));
    }
    else{ //split[0] == "move"
        instructions.emplace_back(move, std::stoi(split[2]), std::stoi(split[5]));
    }
};

auto parseInput = [](const auto& input){
    std::vector<Instruction> instructions{};
    for(const auto& in : input){
        parseIn(in, instructions);
    }
    return instructions;
};

auto scramble = [](std::string s, const auto& instructions){
    for(const auto& instruction : instructions){
        instruction(s);
    }
    return s;
};

auto unscramble = [](std::string s, const auto& instructions){
    for(const auto& instruction : instructions | std::views::reverse){
        instruction(s, true);
    }
    return s;
};



int main(){
    const auto instructions = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto scrambledWord = scramble("abcdefgh", instructions);
    std::cout << "The scrambled word is " << scrambledWord << ".\n";

    //Task 2
    const auto unscrambledWord = unscramble("fbgdceah", instructions);
    std::cout << "The unscrambled version is " << unscrambledWord << ".\n";

    VerifySolution::verifySolution(scrambledWord, unscrambledWord);
}