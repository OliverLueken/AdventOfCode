
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using point = Utilities::Position<int>;
enum Direction{Center = 0, East, SouthEast, SouthWest, West, NorthWest, NorthEast};

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

    int countBlackNeighbors(const point& tile) const {
        int n = 0;
        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                if( (i == -1 && j == 1) || (i == 0 && j == 0) || (i == 1 && j == -1) ) continue;
                point x({tile.first + i, tile.second + j});
                if(blackTiles.find(x) != blackTiles.end()) n++;
            }
        }
        return n;
    }

    void checkWhiteTile(const point& x, std::set<point>& tilesToFlip) const {
        if(blackTiles.find(x) != blackTiles.end()) return;

        int n = countBlackNeighbors(x);
        if(n == 2) tilesToFlip.insert(x);
    }

    void checkNeighbors(const point& tile, std::set<point>& tilesToFlip) const {
        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                if( (i == -1 && j == 1) || (i == 0 && j == 0) || (i == 1 && j == -1) )
                    continue;
                point x = {tile.first + i, tile.second + j};
                checkWhiteTile(x, tilesToFlip);
            }
        }
    }

    auto getTilesToFlip() const {
        auto tilesToFlip = std::set<point>{};
        for(auto& tile : blackTiles){
            int n = countBlackNeighbors(tile);
            if(n == 0 || n > 2) tilesToFlip.insert(tile);

            checkNeighbors(tile, tilesToFlip);
        }
        return tilesToFlip;
    }

    void flipTiles(const std::set<point>& tilesToFlip){
        for(auto& tile : tilesToFlip){
            flipTile(tile);
        }
    }

    void doADay(){
        std::set<point> tilesToFlip = getTilesToFlip();
        flipTiles(tilesToFlip);
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
