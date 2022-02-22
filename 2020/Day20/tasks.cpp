
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_set>

#include "Picture.hpp"
#include "Tile.hpp"

using Position = Utilities::Position<int>;

auto getSnekPos() {
    const auto snek = std::array<std::string, 3>{
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    };

    auto addSnekPositions = [&snek](auto& pos, const auto i){
        size_t p;
        p = snek[i].find('#');
        while (p != std::string::npos) {
            pos.emplace(std::make_pair(i, p));
            p = snek[i].find('#', p + 1);
        }
    };

    std::unordered_set<Position> pos{};
    addSnekPositions(pos, 0);
    addSnekPositions(pos, 1);
    addSnekPositions(pos, 2);
    return pos;
}

bool isSnek(const auto x, const auto y, const auto& snekpos, const auto& water) {
    return std::ranges::all_of(snekpos, [x, y, &water](const auto& pos){
        const auto& [posy, posx] = pos;
        return water(y+posy, x+posx) == '#';
    });
}

int countSnakesInFixedTile(const auto& water, const auto& pos) {
    auto snekCount = 0;
    const auto [n,m] = water.size();
    for (auto y = 0ul; y < n-2; ++y) {
        for (auto x = 0ul; x < m-19; ++x) {
            snekCount+=isSnek(x, y, pos, water);
        }
    }
    return snekCount;
}

auto countSneks(auto& water, const auto& visibleSnekPositions){
    auto rotatingSearch = [](auto& water_, const auto& visibleSnekPositions_){
        int sneks = 0;
        for (int i = 0; i < 4; i++) {
            sneks += countSnakesInFixedTile(water_, visibleSnekPositions_);
            water_.rotateLeft();
        }
        return sneks;
    };

    auto snekCount = 0;
    snekCount += rotatingSearch(water, visibleSnekPositions);
    water.flip();
    snekCount += rotatingSearch(water, visibleSnekPositions);
    return snekCount;
}

auto getWaterRoughness(const auto& pic) {
    auto water = pic.picToTile();
    const auto visibleSnekPositions = getSnekPos();
    const auto snekCount = countSneks(water, visibleSnekPositions);
    return std::ranges::count(water, '#') - visibleSnekPositions.size()*snekCount;
}


auto extractTilesFromInput(auto&& input) {
    auto getTileData = [&input](auto& it){
        const auto cols = it->size();
        std::string tempTileData{};
        auto rows = 0;
        while(!it->empty() && it!=std::end(input)){
            ++rows;
            tempTileData+=std::move(*it);
            ++it;
        }
        return std::make_tuple(rows, cols, std::move(tempTileData));
    };

    std::queue<Tile> tiles;
    for(auto it = std::begin(input); it<=std::end(input); ++it){
        const auto id = std::stoi(Utilities::split(*it)[1]);
        ++it;
        const auto [rows, cols, tempTileData] = getTileData(it);
        tiles.emplace(id, rows, cols, tempTileData);
    }
    return tiles;
}

auto parseInput(auto&& input){
    auto tiles = extractTilesFromInput(input);
    Picture pic{};
    pic.insertTiles(std::move(tiles));
    return pic;
}

int main(){
    const auto pic = parseInput(readFile::vectorOfStrings("input.txt", '\n', true));

    //Task 1
    const auto fourCornerIDsProduct = pic.getCornerProd();
    std::cout << "The product of the IDs of the four corner tiles is " << fourCornerIDsProduct << ".\n";

    //Task 2
    const auto waterRoughness = getWaterRoughness(pic);
    std::cout << "There are " << waterRoughness << " # not part of the sea monster.\n";

    VerifySolution::verifySolution(fourCornerIDsProduct, waterRoughness);
}