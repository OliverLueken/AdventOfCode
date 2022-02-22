#ifndef TILE_HPP
#define TILE_HPP

#include <string>

#include "../../lib/matrix.hpp"


//========================== Tile header start =================================
class Tile : public Matrix::Matrix<char> {
   public:
    int id{};

    Tile() = default;
    Tile(       unsigned long rows, unsigned long cols)                       : Matrix::Matrix<char>(rows, cols),    id(0){};
    Tile(int i, unsigned long rows, unsigned long cols, const std::string& d) : Matrix::Matrix<char>(rows, cols, d), id(i){};

    void rotateLeft();
    void flip();
};
//========================== Tile header end ===================================



#endif