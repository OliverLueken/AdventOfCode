#include "Tile.hpp"

#include <ranges>

void Tile::rotateLeft() {
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

void Tile::flip() {
    const auto [n, m] = size();
    for(auto i = 0u; i<n/2; ++i){
        std::ranges::swap_ranges(row(i), row(n-i-1));
    }
}