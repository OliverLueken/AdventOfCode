
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
#include <unordered_set>
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

struct Game{
    Deck deck1{};
    Deck deck2{};

    Game(const Deck& deck1_, const Deck& deck2_)
        : deck1(deck1_), deck2(deck2_){}

    Game(const Game&) = default;
    Game& operator=(const Game&) = default;
    Game(Game&&) = default;
    Game& operator=(Game&&) = default;
    virtual ~Game() = default;

    auto dealCards(){
        return std::make_pair(
            deck1.deal(),
            deck2.deal()
        );
    }

    auto gameOver() const {
        return deck1.gameOver() || deck2.gameOver();
    }

    virtual Winner getWinner() const {
        return deck2.gameOver() ? Winner::Player1 : Winner::Player2;
    }

    virtual Winner play(){
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

    virtual Winner roundWinner(const unsigned int a, const unsigned int b) const {
        return a > b ? Winner::Player1 : Winner::Player2;
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


auto firstNCards(const Deck& d, const auto n){
    Deck newDeck{};
    std::ranges::copy_n(std::begin(d), n, std::back_inserter(newDeck));
    return newDeck;
}

struct Game2 : public Game{
    std::unordered_set<size_t> existingDecks{};

    Game2(Game game_) : Game(game_){}
    Game2(Deck deck1_, Deck deck2_) : Game{deck1_, deck2_} {}
    Winner roundWinner(const unsigned int a, const unsigned int b) const;
    Winner play();
};

template<>
struct std::hash<Deck>{
    std::size_t operator()(const Deck& deck) const noexcept{
        size_t h = 0;
        auto offset = 0;
        for(const auto& i : deck){
            h+=i<<offset;
            offset=(offset+1)%32;
        }
        return h;
    }
};

template<>
struct std::hash<Game2>{
    std::size_t operator()(const Game2& z) const noexcept{
        const std::size_t h1 = std::hash<Deck>{}(z.deck1);
        const std::size_t h2 = std::hash<Deck>{}(z.deck2);
        return h1^(h2<<1);
    }
};

bool deckAlreadyExisted(auto& game, auto& existingDecks){
    // std::pair<Deck, Deck> p = {deck1, deck2};

    auto it = existingDecks.insert(std::hash<Game2>{}(game));

    return !it.second;
}

Winner Game2::roundWinner(const unsigned int a, const unsigned int b) const {
    if(deck1.size() >= a && deck2.size() >= b){
        Deck deck1copy = firstNCards(deck1, a);
        Deck deck2copy = firstNCards(deck2, b);
        auto nextGame = Game2{deck1copy, deck2copy};
        return nextGame.play();
    }
    return Game::roundWinner(a, b);
}

Winner Game2::play(){
    auto gameWonBy = Winner::NoWinner;

    while(gameWonBy == Winner::NoWinner){
        if(deckAlreadyExisted(*this, existingDecks)){
            gameWonBy = Winner::Player1;
            break;
        }
        const auto [a, b] = dealCards();
        const auto roundWonBy = roundWinner(a, b);
        gameWonBy = updateDeck(*this, roundWonBy, a, b);

    }
    return gameWonBy;
}


Winner playGame2(auto game_, unsigned long& result2, int depth = 0){
    Game2 game{game_};
    const auto gameWonBy = game.play();
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
