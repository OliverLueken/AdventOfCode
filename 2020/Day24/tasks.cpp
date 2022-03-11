
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

typedef std::set<std::string> strset;
typedef std::vector<strset> strsetvec;
typedef std::vector<std::string> strvec;
typedef std::pair<int, int> point;

unsigned long result1 = 0, result2 = 0;

void updateCoords(int direction, point& x){
    switch(direction){
        case 1: {
            x.first += 1;
            x.second += 0;
            break;
        }
        case 2: {
            x.first += 0;
            x.second += -1;
            break;
        }
        case 3: {
            x.first += -1;
            x.second += -1;
            break;
        }
        case 4: {
            x.first += -1;
            x.second += 0;
            break;
        }
        case 5: {
            x.first += 0;
            x.second += 1;
            break;
        }
        case 6: {
            x.first += 1;
            x.second += 1;
            break;
        }
    }
}

int getNextDirection(std::string& line){
    if(line.empty()) return 0;
    char a = line.front();
    line.erase(0, 1);
    if(a == 'e') return 1;
    if(a == 'w') return 4;

    char b = line.front();
    line.erase(0, 1);
    if(a == 'n'){
        if(b == 'w') return 5;
        if(b == 'e') return 6;
    }
    if(b == 'w') return 3;
    // if(b == 'e')
    return 2;
}

void flipTile(std::set<point>& Tiles, const point& x){
    auto it = Tiles.insert(x);
    if(!it.second) Tiles.erase(x);
}

int countBlackNeighbors(const std::set<point>& blackTiles, const point& tile){
    int n = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if( (i == -1 && j == 1) || (i == 0 && j == 0) || (i == 1 && j == -1) )
                continue;
            point x({tile.first + i, tile.second + j});
            if(blackTiles.find(x) != blackTiles.end()) n++;
        }
    }
    return n;
}

void checkWhiteTile(const std::set<point>& blackTiles, const point& x, std::set<point>& tilesToFlip){
    if(blackTiles.find(x) != blackTiles.end()) return;

    int n = countBlackNeighbors(blackTiles, x);
    if(n == 2) tilesToFlip.insert(x);
}

void checkNeighbors(const std::set<point>& blackTiles, const point& tile, std::set<point>& tilesToFlip){
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if( (i == -1 && j == 1) || (i == 0 && j == 0) || (i == 1 && j == -1) )
                continue;
            point x = {tile.first + i, tile.second + j};
            checkWhiteTile(blackTiles, x, tilesToFlip);
        }
    }
}

std::set<point> getTilesToFlip(const std::set<point>& blackTiles){
    std::set<point> tilesToFlip;
    for(auto& tile : blackTiles){
        int n = countBlackNeighbors(blackTiles, tile);
        if(n == 0 || n > 2) tilesToFlip.insert(tile);

        checkNeighbors(blackTiles, tile, tilesToFlip);
    }
    return tilesToFlip;
}

void flipTiles(std::set<point>& blackTiles, const std::set<point>& tilesToFlip){
    for(auto& tile : tilesToFlip){
        flipTile(blackTiles, tile);
    }
}

void doADay(std::set<point>& blackTiles){
    std::set<point> tilesToFlip = getTilesToFlip(blackTiles);
    flipTiles(blackTiles, tilesToFlip);
}

auto buildFloor(auto& input){
    std::set<point> blackTiles;
    for(auto& x : input){
        flipTile(blackTiles, x);
    }
    return blackTiles;
}

auto evolveFloor(auto& blackTiles){
    for(auto i = 0; i < 100; ++i){
        doADay(blackTiles);
    }
    return blackTiles.size();
}

point obtainTileCoords(std::string& line){
    point x = {0, 0};
    int nextDirection;
    do {
        nextDirection = getNextDirection(line);
        updateCoords(nextDirection, x);
    } while(nextDirection > 0);
    return x;
}

auto parseInput(auto&& input){
    auto tilesToFlip = std::set<point>{};
    for(auto& line : input){
        const auto x = obtainTileCoords(line);
        const auto inserted = tilesToFlip.insert(x).second;
        if(!inserted) tilesToFlip.erase(x);
    }
    return tilesToFlip;
}

int main(){
    auto input = parseInput( readFile::vectorOfStrings() );

    //Task 1
    auto blackTiles = buildFloor(input);
    const auto numberOfBlackTiles = blackTiles.size();
    std::cout << "There are " << numberOfBlackTiles << " black tiles.\n";

    //Task 2
    const auto numberOfBlackTilesAfter100Days = evolveFloor(blackTiles);
    std::cout << "After 100 days, there are " << numberOfBlackTiles << " black tiles.\n";

    VerifySolution::verifySolution(numberOfBlackTiles, numberOfBlackTilesAfter100Days);
}
