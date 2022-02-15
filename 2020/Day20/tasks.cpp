
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "Picture.hpp"
#include "Tile.hpp"

using Position = Utilities::Position<int>;

std::vector<std::vector<size_t>> getSnekPos() {
    std::string snek1 = "                  # ";
    std::string snek2 = "#    ##    ##    ###";
    std::string snek3 = " #  #  #  #  #  #   ";

    std::vector<std::vector<size_t>> pos(3);
    size_t p;
    p = snek1.find("#");
    while (p != std::string::npos) {
        pos[0].push_back(p);
        p = snek1.find("#", p + 1);
    }
    p = snek2.find("#");
    while (p != std::string::npos) {
        pos[1].push_back(p);
        p = snek2.find("#", p + 1);
    }
    p = snek3.find("#");
    while (p != std::string::npos) {
        pos[2].push_back(p);
        p = snek3.find("#", p + 1);
    }
    return pos;
}

bool isSnek(auto x, auto y, std::vector<size_t>& snekpos, auto& water) {
    for (size_t i : snekpos) {
        if( water(y, x+i) != '#' ) return false;
    }
    return true;
}

int findSnakes(auto& water) {
    std::vector<std::vector<size_t>> pos = getSnekPos();
    int sneks = 0;
    const auto [n,m] = water.size();
    for (auto y = 0ul; y < n-2; y++) {
        for (auto x = 0ul; x < m - 19; x++) {
            bool isSnake = isSnek(x, y,     pos[0], water) &&
                           isSnek(x, y + 1, pos[1], water) &&
                           isSnek(x, y + 2, pos[2], water);

            if (isSnake) sneks++;
        }
    }
    return sneks;
}

auto getWaterRoughness(auto& pic) {
    auto water = pic.picToTile();
    int sneks = 0;
    for (int i = 0; i < 4; i++) {
        sneks += findSnakes(water);
        water.rotateLeft();
    }
    water.flip();
    for (int i = 0; i < 4; i++) {
        sneks += findSnakes(water);
        water.rotateLeft();
    }
    return std::ranges::count(water, '#') - 15 * sneks;
}


auto extractTilesFromInput(auto&& input) {
    std::queue<tile> tiles;
    for(auto it = std::begin(input); it<=std::end(input); ++it){
        const auto id = std::stoi(Utilities::split(*it)[1]);
        ++it;
        std::string tempTileData{};
        auto rows = 0;
        const auto cols = it->size();
        while(!it->empty() && it!=std::end(input)){
            ++rows;
            tempTileData+=std::move(*it);
            ++it;
        }
        tiles.emplace(id, rows, cols, tempTileData);
    }
    return tiles;
}

auto parseInput(auto&& input){
    auto tiles = extractTilesFromInput(input);
    return picture{std::move(tiles)};
}

int main(){
    auto pic = parseInput(readFile::vectorOfStrings("input.txt", '\n', true));

    //Task 1
    const auto fourCornerIDsProduct = pic.getCornerProd();
    std::cout << "The product of the IDs of the four corner tiles is " << fourCornerIDsProduct << ".\n";

    //Task 2
    const auto waterRoughness = getWaterRoughness(pic);
    std::cout << "There are " << waterRoughness << " # not part of the sea monster .\n";

    VerifySolution::verifySolution(fourCornerIDsProduct, waterRoughness);
}