
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <iostream>
#include <deque>
#include <unordered_set>
#include <string>
#include <vector>

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

    auto score() const {
        auto sum = [worth=0](const auto& score, const auto& newVal) mutable {
            ++worth;
            return score+newVal*worth;
        };
        return std::accumulate(this->rbegin(), this->rend(), 0u, sum);
    }

    void push(const unsigned int a, const unsigned int b){
        this->push_back(a);
        this->push_back(b);
    }
};

struct Game{
    Deck deck1{};
    Deck deck2{};

    Game(Deck&& deck1_, Deck&& deck2_)
        : deck1(std::move(deck1_)), deck2(std::move(deck2_)){}

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

    virtual bool gameOver() {
        return deck1.gameOver() || deck2.gameOver();
    }

    virtual Winner getWinner() const {
        return deck2.gameOver() ? Winner::Player1 : Winner::Player2;
    }

    virtual Winner play(){
        while (!gameOver()){
            const auto [a, b] = dealCards();

            const auto winner = roundWinner(a, b);
            updateDeck(winner, a, b);
        }
        return getWinner();
    }

    auto score(){
        return deck1.score()+deck2.score();
    }

    virtual Winner roundWinner(const unsigned int a, const unsigned int b) const {
        return a > b ? Winner::Player1 : Winner::Player2;
    }

    void updateDeck(const Winner winner, const int a, const int b){
        if(winner == Winner::Player1){
            deck1.push(a, b);
        }
        else{
            deck2.push(b, a);
        }
    }
};

auto dealDeck(const auto& input){
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


struct Game2 : public Game{
    std::unordered_set<size_t> existingDecks{};

    Game2(const Game& game_) : Game(game_){}
    Game2(const Game2& game_, const unsigned int a, const unsigned int b)
        : Game{firstNCards(game_.deck1, a), firstNCards(game_.deck2, b)} {}

    Game2(Deck&& deck1_, Deck&& deck2_) : Game{std::move(deck1_), std::move(deck2_)} {}
    Winner roundWinner(const unsigned int a, const unsigned int b) const;

    bool gameOver() {
        return deck1.gameOver() || deck2.gameOver() || deckAlreadyExisted();
    }

    bool deckAlreadyExisted();

    virtual Winner getWinner() const {
        return deck1.gameOver() ? Winner::Player2 : Winner::Player1;
    }

    Deck firstNCards(const Deck& d, const auto n) const {
        Deck newDeck{};
        std::ranges::copy_n(std::begin(d), n, std::back_inserter(newDeck));
        return newDeck;
    }

    Game2 makeSubGame(const unsigned int a, const unsigned int b) const {
        return Game2{*this, a, b};
    }

    bool recursiveCombatNeeded(const unsigned int a, const unsigned int b) const {
        return deck1.size() >= a && deck2.size() >= b;
    }
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

bool Game2::deckAlreadyExisted(){
    const auto it = existingDecks.insert(std::hash<Game2>{}(*this));
    return !it.second;
}

Winner Game2::roundWinner(const unsigned int a, const unsigned int b) const {
    if(recursiveCombatNeeded(a, b)){
        auto nextGame = makeSubGame(a, b);
        return nextGame.play();
    }
    return Game::roundWinner(a, b);
}

auto playGame(auto game){
    game.play();
    return game.score();
}

int main(){
    const auto game = dealDeck(readFile::vectorOfStrings());

    //Task 1
    const auto firstGameScore = playGame(game);
    std::cout << "The score of the first game's winner is "  << firstGameScore  << ".\n";

    //Task 2
    auto secondGameScore = playGame(Game2{game});
    std::cout << "The score of the second game's winner is " << secondGameScore << ".\n";

    VerifySolution::verifySolution(firstGameScore, secondGameScore);
}
