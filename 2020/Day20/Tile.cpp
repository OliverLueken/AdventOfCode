#include "Tile.hpp"

#include <iostream>

void tile::rotateLeft() {
    strvec newdata(data.size());
    for (auto& s : data) {
        for (auto i = 0ul; i < s.size(); i++) {
            newdata[s.size() - i - 1].push_back(s[i]);
        }
    }
    data = newdata;
}

void tile::flip() {
    strvec newdata;
    for (auto it = data.rbegin(); it != data.rend(); it++)
        newdata.push_back(*it);
    data = newdata;
}

void tile::print() const {
    std::cout << id << std::endl;
    for (auto& a : data) std::cout << a << std::endl;
    std::cout << std::endl;
}