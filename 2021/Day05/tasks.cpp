
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <cmath>

using Position = std::pair<int,int>;
struct positionHash{
    std::size_t operator()(const Position& position) const noexcept{
        return (size_t) position.first << 32 | position.second;
    }
};

auto parseInput = [](const auto& input){
    std::vector<std::pair<Position, Position>> lines{};
    std::ranges::transform(input, std::back_inserter(lines),
        [](const auto& row){
        const auto split = Utilities::splitOnEach(row, " ,");
        return std::make_pair(  std::make_pair(std::stoi(split[0]), std::stoi(split[1])),
                                std::make_pair(std::stoi(split[3]), std::stoi(split[4]))  );
        }
    );
    return lines;
};

auto sign = [](const auto n){
    if( n==0 ) return 0;
    return (int) std::copysign(1, n);
};

auto getOverlaps = [](const auto& lines, const bool includeDiagonals = true){
    std::unordered_map<Position, int, positionHash> overlapCounter{};
    for(const auto& [left, right] : lines){

        const auto direction = std::make_pair( sign(right.first-left.first), sign(right.second-left.second) );

        if(left.first == right.first || left.second==right.second){
            for(auto pos = left; pos!=right; pos.first+=direction.first, pos.second+=direction.second){
                overlapCounter[pos]++;
            }
            overlapCounter[right]++;
        }
        else if( includeDiagonals && std::abs(direction.first) == std::abs(direction.second)){ //diagonal lines
            for(auto pos = left; pos!=right; pos.first+=direction.first, pos.second+=direction.second){
                overlapCounter[pos]++;
            }
            overlapCounter[right]++;
        }
    }

    return std::ranges::count_if( overlapCounter, [](const auto& pair){ return pair.second>=2; }  );
};

int main(){
    const auto lines = parseInput(readFile::vectorOfStrings("input.txt"));

    //Task 1
    const auto overlaps = getOverlaps(lines, false);
    std::cout << "There are " << overlaps << " points the lines overlap.\n";

    //Task 2
    const auto overlapsWithDiagonals = getOverlaps(lines);
    std::cout << "Including diagonal lines, there are " << overlapsWithDiagonals << " points the lines overlap.\n";
}