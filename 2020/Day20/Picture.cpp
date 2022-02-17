
#include "Picture.hpp"


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

void picture::print() const {
    std::cout << "Printing picture\n";
    for (auto& [p, t] : field) {
        std::cout << p.first << ", " << p.second << ": " << t.id << std::endl;
        t.print();
    }
    std::cout << "Printing picture done\n\n";
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

auto fitsWith = [](const auto side, const auto& tile, const auto& pos, const auto& field){
    const auto [n,m] = tile.size();
    switch(side){
    break; case Side::left:{
        const auto neighborPos = Position{pos.first - 1, pos.second    };
        if (field.contains(neighborPos)){
            const auto& neighborTile = field.at(neighborPos);
            return std::ranges::equal( tile.col(0), neighborTile.col(m-1) );
        }}
    break; case Side::right:{
        const auto neighborPos = Position{pos.first + 1, pos.second    };
        if (field.contains(neighborPos)){
            const auto& neighborTile = field.at(neighborPos);
            return std::ranges::equal( tile.col(m-1), neighborTile.col(0) );
        }}
    break; case Side::up:{
        const auto neighborPos = Position{pos.first    , pos.second + 1};
        if (field.contains(neighborPos)){
            const auto& neighborTile = field.at(neighborPos);
            return std::ranges::equal( tile.row(0), neighborTile.row(n-1) );
        }}
    break; case Side::down:{
        const auto neighborPos = Position{pos.first    , pos.second - 1};
        if (field.contains(neighborPos)){
            const auto& neighborTile = field.at(neighborPos);
            return std::ranges::equal( tile.row(n-1), neighborTile.row(0) );
        }}
    }
    return true;
};

bool picture::doesTileFit(const tile& t, const Position& p) const {
    const auto sides = std::array<Side, 4>{Side::left, Side::right, Side::up, Side::down};
    return std::ranges::all_of(sides, [&t, &p, this](const auto& side){
        return fitsWith(side, t, p, this->field);
    });
}


void picture::updateFreePoints(const Position& p) {
    auto isInBounds = [this](const Position& p) {
        if (!this->maxBoundsFound) return true;
        const auto [x, y] = p;
        return Utilities::isBetween(x, this->minx, this->maxx+1)
            && Utilities::isBetween(y, this->miny, this->maxy+1);
    };

    if (!maxBoundsFound && length * height == numberOfTiles) {
        maxBoundsFound = true;
        std::erase_if(freePositions, std::not_fn(isInBounds));
    }

    auto addToFreePositions = [this, &isInBounds](auto&& p){
        if(isInBounds(p)){
            this->freePositions.emplace(std::move(p));
        }
    };

    addToFreePositions( Position{p.first - 1, p.second    } );
    addToFreePositions( Position{p.first + 1, p.second    } );
    addToFreePositions( Position{p.first    , p.second + 1} );
    addToFreePositions( Position{p.first    , p.second - 1} );
}

tile picture::picToTile() const {
    auto [tileHeight, tileLength] = field.begin()->second.size();
    auto water = tile{ (maxy-miny+1)*(tileHeight-2), (maxx-minx+1)*(tileHeight-2) };
    for(auto tiley = maxy; tiley>=miny; --tiley){
        for(auto y = 0ul; y<tileHeight-2; ++y){
            for(auto tilex = minx; tilex<=maxx; ++tilex){
                const auto& tile = field.at({tilex, tiley});
                for(auto x = 0ul; x<tileHeight-2; ++x){
                    water( (maxy-tiley)*(tileHeight-2)+y, (tilex-minx)*(tileHeight-2)+x ) = tile(y+1, x+1);
                }
            }
        }
    }
    return water;
}