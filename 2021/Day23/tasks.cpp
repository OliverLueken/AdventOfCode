
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <ranges>

using Position = Utilities::Position<int>;

struct Amphipod{
    bool moved{false};
    int  type{0};
    Position pos{};

    void set(const bool _moved, const auto& newPos){
        moved = _moved;
        pos = newPos;
    }
};

using Amphipods = std::vector<Amphipod>;


auto hash = [](const Amphipods& amphipods){
    std::string hashVal(50, ' ');
    for(const auto& amphipod : amphipods){
        const auto i = 10*amphipod.pos.x + amphipod.pos.y;
        hashVal[i] = (char)amphipod.type+'A';
    }
    return std::hash<std::string>{}(hashVal);
};

auto parseInput = [](const auto& parsedInput){
    Amphipods amphipods{};
    for(auto row =1; std::less{}(row, parsedInput.size()); row++){
        for(auto col = 1; std::less{}(col, parsedInput[row].size()); col++){
            if(!Utilities::contains(" .#", parsedInput[row][col]) ){
                amphipods.emplace_back(false, parsedInput[row][col]-'A', Position{row-1, col-1});
            }
        }
    }
    return amphipods;
};

auto getGoalHash(const auto& amphipods){
    Amphipods goalAmphipod{amphipods};
    for(auto& amphipod : goalAmphipod){
        amphipod.type = amphipod.pos.y/2-1;
    }
    return hash(goalAmphipod);
}

auto isOccupied(const auto& pos, const auto& amphipods){
    const auto range = amphipods | std::views::transform(&Amphipod::pos);
    return Utilities::contains(range, pos);
}


auto getMaxReachableDistance(const auto& start, const auto& end, const auto& amphipods){
    const auto direction = (end-start)/abs(end-start);
    auto pos = start;
    auto i=0;
    while(pos != end){
        pos=pos+direction;
        if(isOccupied(pos, amphipods)) break;
        i++;
    }
    return i;
}

auto isReachableFrom(const auto& start, const auto& end, const auto& amphipods){
    return getMaxReachableDistance(start, end, amphipods) == abs(end-start);
}

auto moveAmphipod(const auto index, const auto& destinationPosition, const auto& amphipods, auto& next, auto& visited){
    const auto& amphipod = amphipods[index];
    const auto cost = visited[hash(amphipods)]+abs(amphipod.pos-destinationPosition)*(int)std::pow(10,amphipod.type);
    Amphipods newAmphipods{amphipods};
    newAmphipods[index].set(true, destinationPosition);
    const auto h = hash(newAmphipods);
    if( !visited.contains(h) || visited[h] > cost){
        next.emplace_back(std::move(newAmphipods));
        visited[h] = cost;
    }
}

auto moveAmphipodToItsRoom(const size_t index, const Amphipods& amphipods, auto& next, auto& visited){
    const auto& amphipod = amphipods[index];
    const auto goalColumn = (amphipod.type+1)*2;

    //checking if the amphipod's room is reachable
    if(isReachableFrom(amphipod.pos, Position{0, goalColumn}, amphipods) == false) return;

    //move the amphipod as far into it's room as possible
    const auto depth = std::ranges::max(amphipods | std::views::transform(&Amphipod::pos) | std::views::transform(&Position::x));
    auto destinationPosition = Position{depth, goalColumn};
    const auto i = getMaxReachableDistance(Position{0, goalColumn}, destinationPosition, amphipods);

    if(i>0){
        destinationPosition = Position{i, goalColumn};
        moveAmphipod(index, destinationPosition, amphipods, next, visited);
    }
}


auto moveAmphipodOutside(const auto index, const auto& amphipods, auto& next, auto& visited){
    const auto& amphipod = amphipods[index];

    //checking if the outside is reachable for the amphipod
    if(isReachableFrom(amphipod.pos, Position{0, amphipod.pos.y}, amphipods) == false) return;

    //move the amphipod to reachable positions to the left and to the right
    const auto leftFreeUntil  = amphipod.pos.y-getMaxReachableDistance(Position{0, amphipod.pos.y}, Position{0, 0}, amphipods);
    const auto rightFreeUntil = amphipod.pos.y+getMaxReachableDistance(Position{0, amphipod.pos.y}, Position{0,10}, amphipods);

    auto validColumns = std::views::iota(leftFreeUntil, rightFreeUntil+1)
                      | std::views::filter([](const auto i){ return i%2==1 || i==0 || i==10; });
    for(const auto& column : validColumns ){
        const auto destinationPosition = Position{0, column};
        moveAmphipod(index, destinationPosition, amphipods, next, visited);
    }
}

auto addNextMovesForThisAmphipod(const auto index, const auto& amphipods, auto& next, auto& visited){
    if(amphipods[index].pos.x == 0){
        moveAmphipodToItsRoom(index, amphipods, next, visited);
    }
    else if(amphipods[index].moved == false){
        moveAmphipodOutside(index, amphipods, next, visited);
    }
}

auto addAllNextMoves(const Amphipods& amphipods, auto& next, auto& visited){
    for(auto index=0u; index<amphipods.size(); index++){
        addNextMovesForThisAmphipod(index, amphipods, next, visited);
    }
}

auto getLeastEnergyToOrganize = [](const auto& startingAmphipods){
    std::unordered_map<size_t, size_t> visited{};
    visited[hash(startingAmphipods)] = 0;
    std::deque<Amphipods> next{startingAmphipods};
    while(!next.empty()){
        const auto currentAmphipods = next.front();
        next.pop_front();
        addAllNextMoves(currentAmphipods, next, visited);
    }
    return visited[getGoalHash(startingAmphipods)];
};

auto addLines(auto& input){
    input.insert(std::begin(input)+3, "  #D#C#B#A#  ");
    input.insert(std::begin(input)+4, "  #D#B#A#C#  ");
}

int main(){
    auto input = readFile::vectorOfStrings("input.txt");

    //Task 1
    const auto amphipods = parseInput(input);
    const auto result = getLeastEnergyToOrganize(amphipods);
    std::cout << "To organize the amphipods, at least " << result << " energy is required.\n";

    //Task 2
    addLines(input);
    const auto amphipods2 = parseInput(input);
    const auto result2 = getLeastEnergyToOrganize(amphipods2);
    std::cout << "To organize all amphipods, at least " << result2 << " energy is required.\n";

    VerifySolution::verifySolution(result, result2);
}