#ifndef PICTURE_HPP
#define PICTURE_HPP

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

typedef std::vector<std::string> strvec;
typedef std::pair<int, int> point;


class picture {
   public:
    picture(std::queue<tile>&& input);

    long getCornerProd() const;
    void print() const;

    tile picToTile() const;

   private:
    std::map<point, tile> field;
    std::set<point> freepoints;

    int maxx, minx, maxy, miny;
    unsigned long length;
    unsigned long height;
    unsigned long numberOfTiles;
    bool maxBoundsFound;

    bool insertTile(tile& t);
    bool tryInsert(tile& t);
    std::vector<point> getNeighbors(point p) const;
    bool doesTileFit(tile& t, point& tp, point& np) const;
    void updateFreePoints(point& p);
    void printFreepoints() const;
    void updateBounds(point& p);
    bool outOfBounds(point& p) const;
};
//========================== Picture header end ===============================


#endif