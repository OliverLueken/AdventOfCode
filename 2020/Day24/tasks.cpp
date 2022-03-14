
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

using point = Utilities::Position<int>;
enum Direction{Center = 0, East, SouthEast, SouthWest, West, NorthWest, NorthEast};


void updateCoords(auto direction, point& x){
    switch(direction){
    break; case Direction::East:
        x.first  += 1;
        x.second += 0;
    break; case Direction::SouthEast:
        x.first  += 0;
        x.second += -1;
    break; case Direction::SouthWest:
        x.first  += -1;
        x.second += -1;
    break; case Direction::West:
        x.first  += -1;
        x.second += 0;
    break; case Direction::NorthWest:
        x.first  += 0;
        x.second += 1;
    break; case Direction::NorthEast:
        x.first  += 1;
        x.second += 1;
    break; case Direction::Center: break;
    }
}

struct Floor{

    std::set<point> blackTiles{};

    Floor(const auto& tilesToFlip){
        for(const auto& x : tilesToFlip){
            flipTile(x);
        }
    }

    void flipTile(const point& x){
        auto it = blackTiles.insert(x);
        if(!it.second) blackTiles.erase(x);
    }

    void flipTile(const auto& tile, const auto count){
        if(isBlackTile(tile)){
            if(blackTileShouldBeFlipped(count)) blackTiles.erase(tile);
        }
        else{
            if(whiteTileShouldBeFlipped(count)) blackTiles.insert(tile);
        }
    }

    auto ingrementTileNeighborCounts(const auto& tile, auto& neighborCounts) const {
        const auto directions = std::array<Direction, 6>{
            Direction::NorthEast,
            Direction::East,
            Direction::SouthEast,
            Direction::SouthWest,
            Direction::West,
            Direction::NorthWest
        };
        for(const auto direction : directions){
            auto p = tile;
            updateCoords(direction, p);
            neighborCounts[p]++;
        }
    }

    auto getNeighborCounts() const {
        auto neighborCounts = std::unordered_map<point, unsigned int>{};
        std::ranges::transform(blackTiles, std::inserter(neighborCounts, std::begin(neighborCounts)), [](const auto& tile){
            return std::make_pair(tile, 0u);
        }); //We need to insert every black tile, because black tiles need to be flipped if they have 0 black neighbors
            //and the next loop only inserts tiles that have at least one black neighbor
        for(const auto& tile : blackTiles){
            ingrementTileNeighborCounts(tile, neighborCounts);
        }
        return neighborCounts;
    }

    auto isBlackTile(const auto& tile) const {
        return blackTiles.contains(tile);
    }

    bool blackTileShouldBeFlipped(const auto count) const {
        return count == 0 || count > 2;
    }

    bool whiteTileShouldBeFlipped(const auto count) const {
        return count == 2;
    }

    void flipTiles(const auto& neighborCounts){
        for(const auto& [tile, count] : neighborCounts){
            flipTile(tile, count);
        }
    }

    void doADay(){
        const auto neighborCounts = getNeighborCounts();
        flipTiles(neighborCounts);
    }

    auto evolveFloor(){
        for(auto i = 0; i < 100; ++i){
            doADay();
        }
    }

    auto getNumberOfBlackTiles() const {
        return blackTiles.size();
    }
};

auto getNextDirection(auto& line){
    if(line.empty()) return Direction::Center;

    if( line.starts_with('e') ){
        line.remove_prefix(1);
        return Direction::East;
    }
    if( line.starts_with('w') ){
        line.remove_prefix(1);
        return Direction::West;
    }

    if( line.starts_with("nw") ){
        line.remove_prefix(2);
        return Direction::NorthWest;
    }
    if( line.starts_with("ne") ){
        line.remove_prefix(2);
        return Direction::NorthEast;
    }
    if( line.starts_with("sw") ){
        line.remove_prefix(2);
        return Direction::SouthWest;
    }
    line.remove_prefix(2);
    return Direction::SouthEast;
}

point obtainTileCoords(auto&& lineView){
    point x = {0, 0};
    auto nextDirection = Direction::Center;
    do {
        nextDirection = getNextDirection(lineView);
        updateCoords(nextDirection, x);
    } while(nextDirection != Direction::Center);
    return x;
}

auto parseInput(auto&& input){
    auto tilesToFlip = std::set<point>{};
    for(auto& line : input){
        const auto x = obtainTileCoords(std::string_view{line});
        const auto inserted = tilesToFlip.insert(x).second;
        if(!inserted) tilesToFlip.erase(x);
    }
    return tilesToFlip;
}

int main(){
    const auto tilesToFlip = parseInput( readFile::vectorOfStrings() );

    //Task 1
    auto floor = Floor{tilesToFlip};
    const auto numberOfBlackTiles = floor.getNumberOfBlackTiles();
    std::cout << "There are " << numberOfBlackTiles << " black tiles.\n";

    //Task 2
    floor.evolveFloor();
    const auto numberOfBlackTilesAfter100Days = floor.getNumberOfBlackTiles();
    std::cout << "After 100 days, there are " << numberOfBlackTilesAfter100Days << " black tiles.\n";

    VerifySolution::verifySolution(numberOfBlackTiles, numberOfBlackTilesAfter100Days);
}
