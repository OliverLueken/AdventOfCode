
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <ranges>

class Board{
    std::array<int, 25> board{};

public:
    Board(auto&& range){
        auto i=0;
        for(const auto s : range){
            const auto split = Utilities::split(s);
            std::ranges::transform(split, std::begin(board)+5*i,
                [](const auto n){ return std::stoi(n); }
            );
            i++;
        }
    }

    auto mark(int n){
        auto it = std::ranges::find(board, n);
        if(it != std::end(board)){
            *it=-1;  //mark the drawn number n
            const auto pos = (int) std::distance(std::begin(board), it);
            return std::make_pair( true, std::make_pair(pos/5, pos%5) );
        }
        return std::make_pair( false, std::make_pair(0, 0) );
    }

    bool win(const auto& pos) const {
        auto isMarked = [](const int n){ return n==-1; };

        const auto [i, j] = pos;
        const auto row = board | std::views::drop(5*i) | std::views::take(5);
        if( std::ranges::all_of(row, isMarked) ) return true;


        auto everyFifthElement = [i=0](const auto&) mutable {
            return 0 == i++%5;
        };
        auto col = board | std::views::drop(j) | std::views::filter(everyFifthElement);

        return std::ranges::all_of(col, isMarked);
    }

    int score(const auto n) const {
        auto notMarked = [](const auto n){ return n!=-1; };
        return n*Utilities::sum( board | std::views::filter(notMarked) );
    }
};

auto toInts = [](const auto& split){
    std::vector<int> input{};
    std::ranges::transform(split, std::back_inserter(input), [](const auto& s){ return std::stoi(s);});
    return input;
};

auto parseInput = [](const auto& input){
    //parse first row of the input into drawn numbers
    std::vector<int> numbers{toInts(Utilities::split(input[0], ','))};

    //parse every other row into boards
    std::vector<Board> boards{};
    const auto nBoards = std::distance(std::begin(input)+1, std::end(input))/6;

    std::ranges::for_each(std::views::iota(0, nBoards),
        [&input, &boards](const auto boardId){
            boards.emplace_back( input | std::views::drop(2+6*boardId) | std::views::take(5) );
        }
    );

    return std::make_pair(numbers, boards);
};

auto getWinningScores = [](const auto& numbers, auto& boards){
    std::vector<int> winningScores{};

    for(const auto n : numbers){
        for(auto boardIt=std::begin(boards); boardIt!=std::end(boards); ){
            const auto [crossed, crossedPos] = boardIt->mark(n);
            if(crossed && boardIt->win(crossedPos)){
                winningScores.push_back( boardIt->score(n) );
                boardIt = boards.erase(boardIt);
            }
            else{
                boardIt++;
            }
        }
    }

    return std::make_pair(winningScores.front(), winningScores.back());
};


int main(){
    auto [numbers, boards] = parseInput(readFile::vectorOfStrings("input.txt"));

    auto [firstWinningScore, lastWinningScore] = getWinningScores(numbers, boards);

    //Task 1
    std::cout << "The score of the first board to win is " << firstWinningScore << ".\n";

    //Task 2
    std::cout << "The score of the last board to win is "  << lastWinningScore << ".\n";
}