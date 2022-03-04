
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

enum Winner{NoWinner = 0, Player1, Player2};

struct Deck : public std::deque<unsigned int>{
    auto deal(){
        const auto a = front();
        pop_front();
        return a;
    }

    auto gameOver() const {
        return this->size()==0;
    }

    auto score() {
        std::ranges::for_each(this->rbegin(), this->rend(), [worth=0](auto& val) mutable {
            ++worth;
            val*=worth;
        });
        return Utilities::sum(this->begin(), this->end());
    }
};


auto updateDeck(auto& game, const Winner winner, int a, int b){
    if(winner == Winner::Player1){
        game.deck1.push_back(a);
        game.deck1.push_back(b);
        if(game.deck2.empty()){
            return Winner::Player1;
        }
    }
    else{
        game.deck2.push_back(b);
        game.deck2.push_back(a);
        if(game.deck1.empty()){
            return Winner::Player2;
        }
    }
    return Winner::NoWinner;
}

template<typename T>
struct Game{
    Deck deck1{};
    Deck deck2{};
    T roundWinner;

    Game(const Deck& deck1_, const Deck& deck2_, T roundWinner_)
        : deck1(deck1_), deck2(deck2_), roundWinner(roundWinner_){}

    auto dealCards(){
        return std::make_pair(
            deck1.deal(),
            deck2.deal()
        );
    }

    auto gameOver() const {
        return deck1.gameOver() || deck2.gameOver();
    }

    auto getWinner() const {
        return deck2.gameOver() ? Winner::Player1 : Winner::Player2;
    }

    auto play(){
        while (!gameOver()){
            const auto [a, b] = dealCards();

            const auto winner = roundWinner(a, b);
            updateDeck(*this, winner, a, b);
        }
        return getWinner();
    }

    auto score(){
        return deck1.score()+deck2.score();
    }
};

template<typename T, typename S>
bool operator<(const Game<T>& game1, const Game<S>& game2) noexcept{
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
        make_deck(nextDeckIt+1       , std::end(input)),
        [](unsigned int a, unsigned int b){return a > b ? Winner::Player1 : Winner::Player2;}
    };
}

bool deckAlreadyExisted(auto& game, auto& existingDecks){
    // std::pair<Deck, Deck> p = {deck1, deck2};

    auto it = existingDecks.insert(game);

    return !it.second;
}

auto firstNCards(const Deck& d, const auto n){
    Deck newDeck{};
    std::ranges::copy_n(std::begin(d), n, std::back_inserter(newDeck));
    return newDeck;
}

Winner playGame2(auto game, unsigned long& result2, int depth = 0){
    std::set<decltype(game)> existingDecks;
    auto gameWonBy = Winner::NoWinner;

    while(gameWonBy == Winner::NoWinner){

        if(deckAlreadyExisted(game, existingDecks)){
            gameWonBy = Winner::Player1;
            break;
        }

        // Draw
        const auto [a, b] = game.dealCards();


        auto roundWonBy = Winner::NoWinner;
        // Do recursive call?
        if(game.deck1.size() >= a && game.deck2.size() >= b){
            Deck deck1copy = firstNCards(game.deck1, a);
            Deck deck2copy = firstNCards(game.deck2, b);
            auto nextGame = Game{deck1copy, deck2copy, game.roundWinner};
            roundWonBy = playGame2(nextGame, result2, depth + 1);
        }
        else{
            if (a > b)
                roundWonBy = Winner::Player1;
            else
                roundWonBy = Winner::Player2;
        }

        gameWonBy = updateDeck(game, roundWonBy, a, b);
    }

    if(depth == 0){
        result2 = game.score();
    }
    return gameWonBy;
}

auto playGame1(auto game){
    game.play();

    return game.score();
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
