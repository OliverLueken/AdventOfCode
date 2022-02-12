#ifndef PICTURE_HPP
#define PICTURE_HPP

#include "../../lib/utilities.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "Tile.hpp"

using Position = Utilities::Position<int>;


class picture {
   public:
    picture(std::queue<tile>&& input);

    long getCornerProd() const;
    void print() const;

    tile picToTile() const;

   private:
    std::map<Position, tile> field;
    std::set<Position> freePositions;

    int maxx, minx, maxy, miny;
    unsigned long length;
    unsigned long height;
    unsigned long numberOfTiles;
    bool maxBoundsFound;

    bool insertTile(tile& t);
    bool tryInsert(tile& t);
    std::vector<Position> getNeighbors(Position p) const;
    bool doesTileFit(tile& t, Position& tp, Position& np) const;
    void updateFreePoints(Position& p);
    void printFreePositions() const;
    void updateBounds(Position& p);
    bool outOfBounds(Position& p) const;
};
//========================== Picture header end ===============================


#endif