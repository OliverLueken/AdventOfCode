
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
enum class Direction{Center, East, SouthEast, SouthWest, West, NorthWest, NorthEast};


void updateCoords(auto direction, point& x){
    switch(direction){
    break; case Direction::East:
        ++x.x;
    break; case Direction::SouthEast:
        --x.y;
    break; case Direction::SouthWest:
        --x.x;
        --x.y;
    break; case Direction::West:
        --x.x;
    break; case Direction::NorthWest:
        ++x.y;
    break; case Direction::NorthEast:
        ++x.x;
        ++x.y;
    break; case Direction::Center: break;
    }
}

class Floor{

    std::set<point> blackTiles{};

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
            ++neighborCounts[p];
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

    void evolve(){
        const auto neighborCounts = getNeighborCounts();
        flipTiles(neighborCounts);
    }

public:

    void flipTile(point&& x){
        auto it = blackTiles.emplace(std::move(x));
        if(!it.second) blackTiles.erase(it.first);
    }

    auto evolveFloor(){
        for(auto i = 0; i < 100; ++i){
            evolve();
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
    auto floor = Floor{};
    for(auto& line : input){
        auto x = obtainTileCoords(std::string_view{line});
        floor.flipTile(std::move(x));
    }
    return floor;
}

int main(){
    auto floor = parseInput( readFile::vectorOfStrings() );

    //Task 1
    const auto numberOfBlackTiles = floor.getNumberOfBlackTiles();
    std::cout << "There are " << numberOfBlackTiles << " black tiles.\n";

    //Task 2
    floor.evolveFloor();
    const auto numberOfBlackTilesAfter100Days = floor.getNumberOfBlackTiles();
    std::cout << "After 100 days, there are " << numberOfBlackTilesAfter100Days << " black tiles.\n";

    VerifySolution::verifySolution(numberOfBlackTiles, numberOfBlackTilesAfter100Days);
}
