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

    int maxx{0};
    int minx{0};
    int maxy{0};
    int miny{0};
    unsigned long length{1};
    unsigned long height{1};
    unsigned long numberOfTiles;
    bool maxBoundsFound;

    bool insertTile(tile& t);
    bool tryInsert(tile& t);
    std::vector<Position> getNeighbors(const Position& p) const;
    bool doesTileFit(const tile& t, const Position& tp, const Position& np) const;
    void updateFreePoints(const Position& p);
    void printFreePositions() const;
    void updateBounds(const Position& p);
    bool outOfBounds(Position& p) const;
};
//========================== Picture header end ===============================


#endif