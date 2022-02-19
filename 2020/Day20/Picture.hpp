#ifndef PICTURE_HPP
#define PICTURE_HPP

#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>

#include "Tile.hpp"

using Position = Utilities::Position<int>;

class picture {
   public:
    picture() = default;

    void insertTiles(std::queue<tile>&& input);
    long getCornerProd() const;
    tile picToTile() const;

   private:

    bool maxBoundsFound{false};
    int maxx{0};
    int minx{0};
    int maxy{0};
    int miny{0};
    unsigned long length{1};
    unsigned long height{1};
    unsigned long numberOfTiles;

    std::unordered_map<Position, tile> field{};
    std::unordered_set<Position> freePositions{};

    bool insertTile(tile& t);
    bool tryInsert(tile& t);
    bool doesTileFit(const tile& t, const Position& tp) const;
    void updateFreePoints(const Position& p);
    void updateBounds(const Position& p);
};


#endif