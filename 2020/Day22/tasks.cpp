
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <vector>

using strvec    = std::vector<std::string>;
using deck      = std::deque<int>;

void dealDeck(const strvec& input, deck& deck1, deck& deck2){
    auto i = 1u;
    for(; i < input.size(); i++){
        if (input[i].empty()) break;
        deck1.push_back(stoi(input[i]));
    }
    i += 2;
    for(; i < input.size(); i++){
        deck2.push_back(stoi(input[i]));
    }
}

int updateDeck(deck& deck1, deck& deck2, int roundWonBy, int a, int b){
    if(roundWonBy == 1){
        deck1.push_back(a);
        deck1.push_back(b);
        if(deck2.empty()){
            return 1;
        }
    }
    else{
        deck2.push_back(b);
        deck2.push_back(a);
        if(deck1.empty()){
            return 2;
        }
    }
    return 0;
}

bool deckAlreadyExisted(deck& deck1, deck& deck2, std::set<std::pair<deck, deck>>& existingDecks){
    std::pair<deck, deck> p = {deck1, deck2};

    auto it = existingDecks.insert(p);

    return !it.second;
}

deck firstNcards(deck d, auto n){
    while (d.size() > n) d.pop_back();

    return d;
}

int playGame2(deck deck1, deck deck2, unsigned long& result2, int depth = 0){
    std::set<std::pair<deck, deck>> existingDecks;
    unsigned int a, b;
    int gameWonBy = 0;

    while(gameWonBy == 0){

        if(deckAlreadyExisted(deck1, deck2, existingDecks)){
            gameWonBy = 1;
            break;
        }

        // Draw
        a = deck1.front();
        b = deck2.front();
        deck1.pop_front();
        deck2.pop_front();


        int roundWonBy;
        // Do recursive call?
        if(deck1.size() >= a && deck2.size() >= b){
            deck deck1copy = firstNcards(deck1, a);
            deck deck2copy = firstNcards(deck2, b);
            roundWonBy = playGame2(deck1copy, deck2copy, result2, depth + 1);
        }
        else{
            if (a > b)
                roundWonBy = 1;
            else
                roundWonBy = 2;
        }

        gameWonBy = updateDeck(deck1, deck2, roundWonBy, a, b);
    }

    if(depth == 0){
        if(gameWonBy == 2) deck1 = deck2;
        for(auto i = deck1.size(); i > 0; i--){
            result2 += i * deck1.front();
            deck1.pop_front();
        }
    }
    return gameWonBy;
}

auto playGame1(deck deck1, deck deck2){
    int a, b;
    int gameWonBy = 0;
    while (gameWonBy == 0){
        a = deck1.front();
        b = deck2.front();
        deck1.pop_front();
        deck2.pop_front();

        bool player1wonRound = a > b;
        gameWonBy = updateDeck(deck1, deck2, player1wonRound, a, b);
    }

    auto result1 = 0ul;
    if(gameWonBy == 2) deck1 = deck2;
    for(auto i = deck1.size(); i > 0; i--){
        result1 += i * deck1.front();
        deck1.pop_front();
    }
    return result1;
}

auto doStuff(const strvec& input){
    deck deck1, deck2;
    dealDeck(input, deck1, deck2);
    const auto result1 = playGame1(deck1, deck2);
    auto result2 = 0ul;
    playGame2(deck1, deck2, result2);
    return std::make_pair(result1, result2);
}

strvec readfile(std::string file){
    std::string line;
    std::ifstream input(file);
    strvec lines;

    if(input.is_open()){
        while(getline(input, line)){
            lines.push_back(line);
        }
        input.close();
    }
    else{
        std::cout << "Unable to open file\n";
    }
    return lines;
}

int main(){
    strvec input = readfile("input.txt");

    const auto [result1, result2] = doStuff(input);

    std::cout << result1 << "\n";
    std::cout << result2 << "\n";

    VerifySolution::verifySolution(result1, result2);
}
