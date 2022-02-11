
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

typedef std::vector<std::string> strvec;
typedef std::pair<int, int> point;

std::vector<std::vector<size_t>> getSnekPos() {
    std::string snek1 = "                  # ";
    std::string snek2 = "#    ##    ##    ###";
    std::string snek3 = " #  #  #  #  #  #   ";

    std::vector<std::vector<size_t>> pos(3);
    size_t p;
    p = snek1.find("#");
    // std::cout << p << std::endl;
    while (p != std::string::npos) {
        pos[0].push_back(p);
        // std::cout << p << std::endl;
        p = snek1.find("#", p + 1);
    }
    p = snek2.find("#");
    // std::cout << p << std::endl;
    while (p != std::string::npos) {
        pos[1].push_back(p);
        // std::cout << p << std::endl;
        p = snek2.find("#", p + 1);
    }
    p = snek3.find("#");
    // std::cout << p << std::endl;
    while (p != std::string::npos) {
        pos[2].push_back(p);
        // std::cout << p << std::endl;
        p = snek3.find("#", p + 1);
    }
    return pos;
}

bool isSnek(auto x, auto y, std::vector<size_t>& snekpos, tile& water) {
    std::string s = water.data[y].substr(x);

    for (size_t i : snekpos) {
        if (s[i] != '#') return false;
    }
    return true;
}

int findSnakes(tile& water) {
    std::vector<std::vector<size_t>> pos = getSnekPos();
    int sneks = 0;
    for (auto y = 0ul; y < water.data.size() - 2; y++) {
        for (auto x = 0ul; x < water.data[y].size() - 19; x++) {
            // std::cout << "findSnakes: " << x << ", " << y << std::endl;
            bool isSnake = isSnek(x, y, pos[0], water) &&
                           isSnek(x, y + 1, pos[1], water) &&
                           isSnek(x, y + 2, pos[2], water);

            if (isSnake) sneks++;
        }
    }
    return sneks;
}

int countRoughWater(tile& water) {
    int count = 0;
    for (auto y = 0ul; y < water.data.size(); y++) {
        std::string s = water.data[y];
        size_t p = s.find("#");
        while (p != std::string::npos) {
            count++;
            p = s.find("#", p + 1);
        }
    }
    return count;
}

auto getWaterRoughness(auto& pic) {
    tile water = pic.picToTile();
    water.print();
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
    return countRoughWater(water) - 15 * sneks;
}

int main(){
    auto pic = picture{readFile::vectorOfStrings("input.txt", '\n', true)};

    //Task 1
    const auto fourCornerIDsProduct = pic.getCornerProd();
    std::cout << "The product of the IDs of the four corner tiles is " << fourCornerIDsProduct << ".\n";

    //Task 2
    const auto waterRoughness = getWaterRoughness(pic);
    std::cout << "There are " << waterRoughness << " # not part of the sea monster .\n";

    VerifySolution::verifySolution(fourCornerIDsProduct, waterRoughness);
}