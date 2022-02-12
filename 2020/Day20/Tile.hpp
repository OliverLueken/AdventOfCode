#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <vector>


//========================== Tile header start =================================
class tile {
   public:
    std::vector<std::string> data;
    int id;

    tile() : data(), id(){};
    tile(int i, std::vector<std::string>& d) : data(d), id(i){};

    void rotateLeft();
    void flip();
    void print() const;
};
//========================== Tile header end ===================================



#endif