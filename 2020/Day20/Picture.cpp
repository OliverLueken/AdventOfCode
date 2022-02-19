
#include "Picture.hpp"

#include <ranges>

picture::picture(std::queue<tile>&& tiles) {
    numberOfTiles = tiles.size();
    maxBoundsFound = false;

    while (!tiles.empty()) {
        auto tile = tiles.front();
        tiles.pop();
        const auto inserted = insertTile(tile);
        if (!inserted) {
            tiles.push(std::move(tile));
        }
    }
}

long picture::getCornerProd() const {
    long sum = 1;
    sum *= field.at({minx, miny}).id;
    sum *= field.at({maxx, miny}).id;
    sum *= field.at({minx, maxy}).id;
    sum *= field.at({maxx, maxy}).id;
    return sum;
}

bool picture::insertTile(tile& tile) {

    if (field.empty()) {
        const auto origin = Position{0, 0};
        field[origin] = tile;
        updateFreePoints(origin);
        return true;
    }
    for (int i = 0; i < 4; i++) {
        bool inserted = tryInsert(tile);
        if (inserted) return true;
        tile.rotateLeft();
    }
    tile.flip();
    for (int i = 0; i < 4; i++) {
        bool inserted = tryInsert(tile);
        if (inserted) return true;
        tile.rotateLeft();
    }
    return false;
}

bool picture::tryInsert(tile& tile) {
    for (const auto& p : freePositions) {
        const auto tileFits = doesTileFit(tile, p);
        if (tileFits) {
            field[p] = std::move(tile);
            updateBounds(p);
            updateFreePoints(p);
            freePositions.erase(p);
            return true;
        }
    }
    return false;
}

void picture::updateBounds(const Position& p) {
    int x = p.first, y = p.second;
    minx = std::min(minx, x);
    maxx = std::max(maxx, x);
    miny = std::min(miny, y);
    maxy = std::max(maxy, y);
    length = maxx - minx + 1;
    height = maxy - miny + 1;
}

enum Side{left, right, up, down};

template<Side>
auto fitsWith = [](const auto& tile, const auto& pos, const auto& field){
    return true;
};

template<>
auto fitsWith<Side::left> = [](const auto& tile, const auto& pos, const auto& field){
    const auto [n,m] = tile.size();
    const auto neighborPos = Position{pos.first - 1, pos.second    };
    if (field.contains(neighborPos)){
        const auto& neighborTile = field.at(neighborPos);
        return std::ranges::equal( tile.col(0), neighborTile.col(m-1) );
    }
    return true;
};

template<>
auto fitsWith<Side::right> = [](const auto& tile, const auto& pos, const auto& field){
    const auto [n,m] = tile.size();
    const auto neighborPos = Position{pos.first + 1, pos.second    };
    if (field.contains(neighborPos)){
        const auto& neighborTile = field.at(neighborPos);
        return std::ranges::equal( tile.col(m-1), neighborTile.col(0) );
    }
    return true;
};

template<>
auto fitsWith<Side::up> = [](const auto& tile, const auto& pos, const auto& field){
    const auto [n,m] = tile.size();
    const auto neighborPos = Position{pos.first    , pos.second + 1};
    if (field.contains(neighborPos)){
        const auto& neighborTile = field.at(neighborPos);
        return std::ranges::equal( tile.row(0), neighborTile.row(n-1) );
    }
    return true;
};

template<>
auto fitsWith<Side::down> = [](const auto& tile, const auto& pos, const auto& field){
    const auto [n,m] = tile.size();
    const auto neighborPos = Position{pos.first    , pos.second - 1};
    if (field.contains(neighborPos)){
        const auto& neighborTile = field.at(neighborPos);
        return std::ranges::equal( tile.row(n-1), neighborTile.row(0) );
    }
    return true;
};

bool picture::doesTileFit(const tile& t, const Position& p) const {
    return fitsWith<Side::left >(t, p, this->field)
        && fitsWith<Side::right>(t, p, this->field)
        && fitsWith<Side::up   >(t, p, this->field)
        && fitsWith<Side::down >(t, p, this->field);
}


void picture::updateFreePoints(const Position& p) {
    auto isInBounds = [this](const Position& pos) {
        if (!this->maxBoundsFound) return true;
        const auto [x, y] = pos;
        return Utilities::isBetween(x, this->minx, this->maxx+1)
            && Utilities::isBetween(y, this->miny, this->maxy+1);
    };

    if (!maxBoundsFound && length*height == numberOfTiles) {
        maxBoundsFound = true;
        std::erase_if(freePositions, std::not_fn(isInBounds));
    }

    auto addToFreePositions = [this, &isInBounds](auto&& pos){
        if(isInBounds(pos)){
            this->freePositions.emplace(std::move(pos));
        }
    };

    addToFreePositions( Position{p.first - 1, p.second    } );
    addToFreePositions( Position{p.first + 1, p.second    } );
    addToFreePositions( Position{p.first    , p.second + 1} );
    addToFreePositions( Position{p.first    , p.second - 1} );
}

tile picture::picToTile() const {
    auto [tileHeight, tileLength] = field.begin()->second.size();
    auto water = tile{ (maxy-miny+1)*(tileHeight-2), (maxx-minx+1)*(tileLength-2) };
    for(const auto& [tilePos, tile] : field){
        const auto& [tilex, tiley] = tilePos;
        const auto yoffset = (maxy-tiley)*(tileHeight-2)-1;
        const auto xoffset = (tilex-minx)*(tileLength-2)-1;
        for(auto y = 1ul; y<tileHeight-1; ++y){
            for(auto x = 1ul; x<tileLength-1; ++x){
                water( yoffset+y, xoffset+x ) = tile(y, x);
            }
        }
    }
    return water;
}