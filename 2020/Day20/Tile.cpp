#include "Tile.hpp"

#include <iostream>

void tile::rotateLeft() {
    const auto [n, m] = size();
    Matrix<char> newData{m, n};
    for(auto i = 0u; i<m; ++i){
        for(auto j=0u; j<n; ++j){
            newData(m-1-i, j) = this->operator()(j, i);
        }
    }
    std::swap(data(), newData.data());
}

void tile::flip() {
    const auto [n, m] = size();
    for(auto i = 0u; i<n/2; ++i){
        for(auto j=0u; j<m; ++j){
            std::swap(this->operator()(i,j), this->operator()(n-1-i, j));
        }
    }
}

void tile::print() const {
    std::cout << id << std::endl;
    const auto [n, m] = size();
    for(auto i=0u; i<n; ++i){
        for(auto j=0u; j<m; ++j){
            std::cout << this->operator()(i,j);
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}