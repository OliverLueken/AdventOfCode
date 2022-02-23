
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

struct Deck : public std::deque<unsigned int>{
    auto deal(){
        const auto a = front();
        pop_front();
        return a;
    }
};

struct Game{
    Deck deck1{};
    Deck deck2{};

    auto dealCards(){
        return std::make_pair(
            deck1.deal(),
            deck2.deal()
        );
    }
};

bool operator<(const Game& game1, const Game& game2) noexcept{
    return game1.deck1 < game2.deck1 || game1.deck2 < game2.deck2;
}

auto dealDeck(const strvec& input){
    auto make_deck = [](auto begin, auto end){
        auto toUInt = [](const auto& s){return static_cast<unsigned int>(std::stoi(s));};
        auto deck = Deck{};
        std::ranges::transform(begin, end, std::back_inserter(deck), toUInt);
        return deck;
    };

    const auto nextDeckIt = std::ranges::find(input, "Player 2:");
    return Game{
        make_deck(std::begin(input)+1, nextDeckIt),
        make_deck(nextDeckIt+1       , std::end(input))
    };
}

int updateDeck(auto& game, int roundWonBy, int a, int b){
    if(roundWonBy == 1){
        game.deck1.push_back(a);
        game.deck1.push_back(b);
        if(game.deck2.empty()){
            return 1;
        }
    }
    else{
        game.deck2.push_back(b);
        game.deck2.push_back(a);
        if(game.deck1.empty()){
            return 2;
        }
    }
    return 0;
}

bool deckAlreadyExisted(auto& game, auto& existingDecks){
    // std::pair<Deck, Deck> p = {deck1, deck2};

    auto it = existingDecks.insert(game);

    return !it.second;
}

Deck firstNcards(Deck d, auto n){
    while (d.size() > n) d.pop_back();

    return d;
}

int playGame2(auto game, unsigned long& result2, int depth = 0){
    std::set<Game> existingDecks;
    int gameWonBy = 0;

    while(gameWonBy == 0){

        if(deckAlreadyExisted(game, existingDecks)){
            gameWonBy = 1;
            break;
        }

        // Draw
        const auto [a, b] = game.dealCards();


        int roundWonBy;
        // Do recursive call?
        if(game.deck1.size() >= a && game.deck2.size() >= b){
            Deck deck1copy = firstNcards(game.deck1, a);
            Deck deck2copy = firstNcards(game.deck2, b);
            auto nextGame = Game{deck1copy, deck2copy};
            roundWonBy = playGame2(nextGame, result2, depth + 1);
        }
        else{
            if (a > b)
                roundWonBy = 1;
            else
                roundWonBy = 2;
        }

        gameWonBy = updateDeck(game, roundWonBy, a, b);
    }

    if(depth == 0){
        if(gameWonBy == 2) game.deck1 = game.deck2;
        for(auto i = game.deck1.size(); i > 0; i--){
            result2 += i * game.deck1.front();
            game.deck1.pop_front();
        }
    }
    return gameWonBy;
}

auto playGame1(auto game){
    int gameWonBy = 0;
    while (gameWonBy == 0){
        const auto [a, b] = game.dealCards();

        bool player1wonRound = a > b;
        gameWonBy = updateDeck(game, player1wonRound, a, b);
    }

    auto result1 = 0ul;
    if(gameWonBy == 2) game.deck1 = game.deck2;
    for(auto i = game.deck1.size(); i > 0; i--){
        result1 += i * game.deck1.front();
        game.deck1.pop_front();
    }
    return result1;
}

int main(){
    const auto game = dealDeck(readFile::vectorOfStrings());

    //Task 1
    const auto result1 = playGame1(game);
    std::cout << result1 << "\n";

    //Task 2
    auto result2 = 0ul;
    playGame2(game, result2);
    std::cout << result2 << "\n";

    VerifySolution::verifySolution(result1, result2);
}
