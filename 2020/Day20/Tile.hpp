#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <vector>

typedef std::vector<std::string> strvec;


//========================== Tile header start =================================
class tile {
   public:
    strvec data;
    int id;

    tile() : data(), id(){};
    tile(int i, strvec& d) : data(d), id(i){};

    void rotateLeft();
    void flip();
    void print() const;
};
//========================== Tile header end ===================================



#endif