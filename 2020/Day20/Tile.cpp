#include "Tile.hpp"

#include <ranges>

void tile::rotateLeft() {
    const auto [n, m] = size();
    auto newData = Matrix<char>{m, n};
    for(auto i = 0u; i<m; ++i){
        std::ranges::copy(
            this->row(i) | std::views::reverse,
            newData.col(i).begin()
        );
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